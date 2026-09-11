#include <cahute.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

static bool is_g3a(const uint8_t *buf, long len)
{
    if (len < 0x7000)
        return false;

    uint8_t inv[0x20];
    for (size_t i = 0; i < sizeof(inv); i++)
        inv[i] = (uint8_t)~buf[i];
    if (memcmp(inv, "USBPower", 8) != 0)
        return false;
    return inv[8] == 0x2c;
}

static int wait_for_handshake(cahute_link *link)
{
    cahute_u8 buf[18];
    size_t received;
    int err;

    while (1) {
        err = cahute_receive_on_link(link, buf, sizeof(buf) - 1, 5000, 5000);
        if (err == CAHUTE_ERROR_TIMEOUT)
            continue;
        if (err)
            return err;

        buf[sizeof(buf) - 1] = 0;
        if (strcmp((char *)buf, "USB loader ready") == 0) {
            printf("Ready to send!\n");
            return 0;
        }

        fprintf(stderr, "error: Unknown message received: %s\n", buf);
        return CAHUTE_ERROR_INVALID;
    }
}

static int send_file(cahute_link *link, const cahute_u8 *data, long size)
{
    cahute_u8 sizebuf[4];
    sizebuf[0] = (size >> 24) & 0xFF;
    sizebuf[1] = (size >> 16) & 0xFF;
    sizebuf[2] = (size >> 8) & 0xFF;
    sizebuf[3] = size & 0xFF;

    int err = cahute_send_on_link(link, sizebuf, sizeof(sizebuf));
    if (err) {
        fprintf(stderr, "error: Cannot send size: %s\n", cahute_get_error_name(err));
        return err;
    }

    printf("Sending %ld bytes\n", size);

    err = cahute_send_on_link(link, data, size);
    if (err) {
        fprintf(stderr, "error: Cannot send data: %s\n", cahute_get_error_name(err));
        return err;
    }

    printf("Sent %ld bytes\n", size);
    return 0;
}

static char *get_file_path(int argc, char **argv)
{
    if (argc >= 2)
        return argv[1];

    char *line = malloc(FILENAME_MAX);
    if (!line || !fgets(line, FILENAME_MAX, stdin)) {
        free(line);
        return NULL;
    }

    if (!strchr(line, '\n') && !feof(stdin)) {
        free(line);
        return NULL;
    }

    line[strcspn(line, "\r\n")] = 0;
    size_t nread = strlen(line);

    /* Strip surrounding quotes */
    if (nread >= 2 && ((line[0] == '"' && line[nread - 1] == '"') ||
                       (line[0] == '\'' && line[nread - 1] == '\''))) {
        line[nread - 1] = 0;
        memmove(line, line + 1, nread - 1);
    }

    return line;
}

int main(int argc, char **argv)
{
    fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
    fprintf(stderr, "   or drag and drop a file to the console\n");

    char *file_path = get_file_path(argc, argv);
    FILE *fp = fopen(file_path, "rb");
    if (!fp) {
        fprintf(stderr, "error: Unable to open file %s\n", file_path);
        if (argc < 2)
            free(file_path);
        return 1;
    }

    fseek(fp, 0, SEEK_END);
    long fsize = ftell(fp);
    if (fsize > 6 * 1024 * 1024) {
        fprintf(stderr, "error: File is too large (max 6MB)\n");
        fclose(fp);
        if (argc < 2)
            free(file_path);
        return 1;
    }
    fseek(fp, 0, SEEK_SET);

    cahute_u8 *filebuf = malloc(fsize);
    if (!filebuf) {
        fprintf(stderr, "error: Out of memory\n");
        fclose(fp);
        if (argc < 2)
            free(file_path);
        return 1;
    }

    fread(filebuf, fsize, 1, fp);
    fclose(fp);

    if (argc < 2)
        free(file_path);

    cahute_u8 *payload = filebuf;
    if (is_g3a(filebuf, fsize)) {
        printf("Detected G3A add-in header; stripping 0x7000-byte prefix before sending.\n");
        payload = filebuf + 0x7000;
        fsize -= 0x7000;
    }

    cahute_context *ctx = NULL;
    int err = cahute_create_context(&ctx);
    if (err) {
        fprintf(stderr, "error: Cannot create context: %s\n", cahute_get_error_name(err));
        free(filebuf);
        return 1;
    }

    cahute_link *link = NULL;
    err = cahute_open_simple_usb_link(ctx, &link, CAHUTE_USB_NOPROTO | CAHUTE_USB_FILTER_SERIAL);
    if (err) {
        fprintf(stderr, "error: Cannot open USB link: %s\n", cahute_get_error_name(err));
        cahute_destroy_context(ctx);
        free(filebuf);
        return 1;
    }

    err = wait_for_handshake(link);
    if (!err)
        err = send_file(link, payload, fsize);

    cahute_close_link(link);
    cahute_destroy_context(ctx);
    free(filebuf);

    return err;
}
