/* User-supplied DAT bundle; each DAT is exposed as a separate read-only stream. */
#include "pocket_assets.h"
#include <of_file.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#define MAX_ASSET_BYTES (16u * 1024u * 1024u)
static unsigned char *bundle;
static size_t bundle_size;
static uint32_t entry_count;
static uint32_t get_u32(const unsigned char *p) {
    return (uint32_t)p[0] | (uint32_t)p[1]<<8 | (uint32_t)p[2]<<16 | (uint32_t)p[3]<<24;
}
int pocket_assets_init(void) {
#ifndef OF_PC
    of_file_slot_register(4, "popdata.dat");
#endif
    FILE *f = fopen("popdata.dat", "rb");
    if (!f) return -1;
    if (fseek(f, 0, SEEK_END) || (bundle_size = (size_t)ftell(f)) < 12 || bundle_size > MAX_ASSET_BYTES || fseek(f, 0, SEEK_SET)) { fclose(f); return -1; }
    bundle = malloc(bundle_size);
    if (!bundle) { fclose(f); return -1; }
    if (fread(bundle, 1, bundle_size, f) != bundle_size) { fclose(f); free(bundle); bundle = NULL; return -1; }
    fclose(f);
    if (memcmp(bundle, "POPDAT01", 8)) goto invalid;
    entry_count = get_u32(bundle + 8);
    if (entry_count > (bundle_size - 12) / 24) goto invalid;
    for (uint32_t i = 0; i < entry_count; ++i) {
        const unsigned char *e = bundle + 12 + i*24;
        uint32_t offset = get_u32(e+16), size = get_u32(e+20);
        if (!memchr(e, 0, 16) || offset < 12 + entry_count*24 || offset > bundle_size || size > bundle_size-offset) goto invalid;
    }
    return 0;
invalid:
    free(bundle); bundle = NULL; return -1;
}
FILE *pocket_open_dat(const char *filename) {
    if (!bundle) return NULL;
    for (uint32_t i = 0; i < entry_count; ++i) {
        const unsigned char *e = bundle + 12 + i*24;
        if (!strcasecmp(filename, (const char *)e))
            return fmemopen(bundle + get_u32(e+16), get_u32(e+20), "rb");
    }
    return NULL;
}
