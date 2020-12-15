#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef float f32;

static bool read_file(const char *path, u32 *size, u8 **data) {
        bool ret = false;

        FILE *input = fopen(path, "r");
        if (!input) {
                return ret;
        }

        if (fseek(input, 0, SEEK_END)) {
                goto cleanup;
        }

        long pos = ftell(input);
        if (pos < 0 || (u32)pos > UINT32_MAX) {
                goto cleanup;
        }
        *size = pos;

        if (fseek(input, 0, SEEK_SET)) {
                goto cleanup;
        }

        *data = malloc(*size * sizeof(u8));
        if (!*data) {
                goto cleanup;
        }

        if (fread(*data, sizeof(u8), *size, input) != *size) {
                goto cleanup;
        }

        ret = true;

cleanup:
        fclose(input);

        return ret;
}

static const char *const vehicle_file_names[0x24] = {
        [0x00] = "sdf_kart.bsp",
        [0x01] = "mdf_kart.bsp",
        [0x02] = "ldf_kart.bsp",
        [0x03] = "sa_kart.bsp",
        [0x04] = "ma_kart.bsp",
        [0x05] = "la_kart.bsp",
        [0x06] = "sb_kart.bsp",
        [0x07] = "mb_kart.bsp",
        [0x08] = "lb_kart.bsp",
        [0x09] = "sc_kart.bsp",
        [0x0a] = "mc_kart.bsp",
        [0x0b] = "lc_kart.bsp",
        [0x0c] = "sd_kart.bsp",
        [0x0d] = "md_kart.bsp",
        [0x0e] = "ld_kart.bsp",
        [0x0f] = "se_kart.bsp",
        [0x10] = "me_kart.bsp",
        [0x11] = "le_kart.bsp",
        [0x12] = "sdf_bike.bsp",
        [0x13] = "mdf_bike.bsp",
        [0x14] = "ldf_bike.bsp",
        [0x15] = "sa_bike.bsp",
        [0x16] = "ma_bike.bsp",
        [0x17] = "la_bike.bsp",
        [0x18] = "sb_bike.bsp",
        [0x19] = "mb_bike.bsp",
        [0x1a] = "lb_bike.bsp",
        [0x1b] = "sc_bike.bsp",
        [0x1c] = "mc_bike.bsp",
        [0x1d] = "lc_bike.bsp",
        [0x1e] = "sd_bike.bsp",
        [0x1f] = "md_bike.bsp",
        [0x20] = "ld_bike.bsp",
        [0x21] = "se_bike.bsp",
        [0x22] = "me_bike.bsp",
        [0x23] = "le_bike.bsp",
};

static const char *const vehicle_names[0x24] = {
        [0x00] = "Standard Kart S",
        [0x01] = "Standard Kart M",
        [0x02] = "Standard Kart L",
        [0x03] = "Booster Seat",
        [0x04] = "Classic Dragster",
        [0x05] = "Offroader",
        [0x06] = "Mini Beast",
        [0x07] = "Wild Wing",
        [0x08] = "Flame Flyer",
        [0x09] = "Cheep Charger",
        [0x0a] = "Super Blooper",
        [0x0b] = "Piranha Prowler",
        [0x0c] = "Tiny Titan",
        [0x0d] = "Daytripper",
        [0x0e] = "Jetsetter",
        [0x0f] = "Blue Falcon",
        [0x10] = "Sprinter",
        [0x11] = "Honeycoupe",
        [0x12] = "Standard Bike S",
        [0x13] = "Standard Bike M",
        [0x14] = "Standard Bike L",
        [0x15] = "Bullet Bike",
        [0x16] = "Mach Bike",
        [0x17] = "Flame Runner",
        [0x18] = "Bit Bike",
        [0x19] = "Sugarscoot",
        [0x1a] = "Wario Bike",
        [0x1b] = "Quacker",
        [0x1c] = "Zip Zip",
        [0x1d] = "Shooting Star",
        [0x1e] = "Magikruiser",
        [0x1f] = "Sneakster",
        [0x20] = "Spear",
        [0x21] = "Jet Bubble",
        [0x22] = "Dolphin Dasher",
        [0x23] = "Phantom",
};

static void get_vehicle_name(const char *path) {
        const char *file_name = path;
        for (const char *c = path; *c; c++) {
                if (*c == '/') {
                        file_name = c + 1;
                }
        }

        for (u32 i = 0; i < 0x24; i++) {
                if (!strcmp(file_name, vehicle_file_names[i])) {
                        printf("\033[1m%s\033[0m (%s):\n", vehicle_names[i], file_name);
                        return;
                }
        }
        printf("\033[1mUnknown vehicle\033[0m (%s):\n", file_name);
}

static u16 get_u16(const u8 *data) {
        return (data[0] << 8) | data[1];
}

static u32 get_u32(const u8 *data) {
        return (data[0] << 24) | (data[1] << 16) | (data[2] << 8) | data[3];
}

static float get_f32(const u8 *data) {
        u32 u = get_u32(data);
        float f;
        memcpy(&f, &u, sizeof(float));
        return f;
}

static void get_hitbox(const u8 *data, u32 hitbox_idx) {
	printf("Hitbox %u enable: %u\n", hitbox_idx, get_u16(data + 0x0));
        if (!get_u16(data + 0x0)) {
                return;
        }
	printf("    X: %f\n", get_f32(data + 0x4));
	printf("    Y: %f\n", get_f32(data + 0x8));
	printf("    Z: %f\n", get_f32(data + 0xc));
	printf("    Radius: %f\n", get_f32(data + 0x10));
        printf("    Unknown 0x14: %u\n", get_u16(data + 0x14));
        printf("    Unknown 0x16: %u\n", get_u16(data + 0x16));
}

static void get_inertia_tensor_cuboid(const u8 *data, u32 cuboid_idx) {
        printf("Inertia tensor cuboid %u\n", cuboid_idx);
	printf("    X: %f\n", get_f32(data + 0x0));
	printf("    Y: %f\n", get_f32(data + 0x4));
	printf("    Z: %f\n", get_f32(data + 0x8));
}

static void get_wheel(const u8 *data, u32 wheel_idx) {
        printf("Wheel %u enable: %u\n", wheel_idx, get_u16(data + 0x0));
        if (!get_u16(data + 0x0)) {
                return;
        }
        printf("    Distance suspension factor: %f\n", get_f32(data + 0x4));
        printf("    Speed suspension factor: %f\n", get_f32(data + 0x8));
        printf("    Y slack: %f\n", get_f32(data + 0xc));
        printf("    X: %f\n", get_f32(data + 0x10));
        printf("    Y: %f\n", get_f32(data + 0x14));
        printf("    Z: %f\n", get_f32(data + 0x18));
        printf("    X rotation: %f\n", get_f32(data + 0x1c));
        printf("    Wheel radius: %f\n", get_f32(data + 0x20));
        printf("    Hitbox radius: %f\n", get_f32(data + 0x24));
        printf("    Unknown 0x28: %u\n", get_u32(data + 0x28));
}

int main(int argc, char **argv) {
        if (argc < 2) {
                fprintf(stderr, "Usage: mkw-bsp-parser FILES...\n");
                return 1;
        }

        for (int i = 1; i < argc; i++) {
                u32 size;
                u8 *data;
                if (!read_file(argv[i], &size, &data)) {
                        continue;
                }

                if (size != 0x25c) {
                        free(data);
                        continue;
                }

                get_vehicle_name(argv[i]);

                u8 *ptr = data;

                printf("Initial position: %f\n", get_f32(ptr));
                ptr += 0x4;

                for (u32 j = 0; j < 16; j++) {
                        get_hitbox(ptr, j);
                        ptr += 0x18;
                }

                for (u32 j = 0; j < 2; j++) {
                        get_inertia_tensor_cuboid(ptr, j);
                        ptr += 0xc;
                }

                printf("Rotation speed: %f\n", get_f32(ptr));
                ptr += 0x4;
                printf("Unknown 0x1a0: %f\n", get_f32(ptr));
                ptr += 0x4;

                for (u32 j = 0; j < 4; j++) {
                        get_wheel(ptr, j);
                        ptr += 0x2c;
                }

                printf("Unknown 0x254: %f\n", get_f32(ptr));
                ptr += 0x4;
                printf("Unknown 0x258: %f\n", get_f32(ptr));

                free(data);
        }

        return 0;
}
