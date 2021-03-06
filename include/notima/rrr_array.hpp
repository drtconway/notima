#ifndef NOTIMA_RRR_ARRAY_HPP
#define NOTIMA_RRR_ARRAY_HPP

#include <iostream>

namespace notima
{
    namespace detail
    {
        
        static constexpr uint64_t* binom[] =
            {
                (uint64_t[]){0x0ull},
                (uint64_t[]){0x1ull, 0x1ull},
                (uint64_t[]){0x1ull, 0x2ull, 0x1ull},
                (uint64_t[]){0x1ull, 0x3ull, 0x3ull, 0x1ull},
                (uint64_t[]){0x1ull, 0x4ull, 0x6ull, 0x4ull, 0x1ull},
                (uint64_t[]){0x1ull, 0x5ull, 0xaull, 0xaull, 0x5ull, 0x1ull},
                (uint64_t[]){0x1ull, 0x6ull, 0xfull, 0x14ull, 0xfull, 0x6ull, 0x1ull},
                (uint64_t[]){0x1ull, 0x7ull, 0x15ull, 0x23ull, 0x23ull, 0x15ull, 0x7ull, 0x1ull},
                (uint64_t[]){0x1ull, 0x8ull, 0x1cull, 0x38ull, 0x46ull, 0x38ull, 0x1cull, 0x8ull, 0x1ull},
                (uint64_t[]){0x1ull, 0x9ull, 0x24ull, 0x54ull, 0x7eull, 0x7eull, 0x54ull, 0x24ull, 0x9ull, 0x1ull},
                (uint64_t[]){0x1ull, 0xaull, 0x2dull, 0x78ull, 0xd2ull, 0xfcull, 0xd2ull, 0x78ull, 0x2dull, 0xaull, 0x1ull},
                (uint64_t[]){0x1ull, 0xbull, 0x37ull, 0xa5ull, 0x14aull, 0x1ceull, 0x1ceull, 0x14aull, 0xa5ull, 0x37ull, 0xbull, 0x1ull},
                (uint64_t[]){0x1ull, 0xcull, 0x42ull, 0xdcull, 0x1efull, 0x318ull, 0x39cull, 0x318ull, 0x1efull, 0xdcull, 0x42ull, 0xcull, 0x1ull},
                (uint64_t[]){0x1ull, 0xdull, 0x4eull, 0x11eull, 0x2cbull, 0x507ull, 0x6b4ull, 0x6b4ull, 0x507ull, 0x2cbull, 0x11eull, 0x4eull, 0xdull, 0x1ull},
                (uint64_t[]){0x1ull, 0xeull, 0x5bull, 0x16cull, 0x3e9ull, 0x7d2ull, 0xbbbull, 0xd68ull, 0xbbbull, 0x7d2ull, 0x3e9ull, 0x16cull, 0x5bull, 0xeull, 0x1ull},
                (uint64_t[]){0x1ull, 0xfull, 0x69ull, 0x1c7ull, 0x555ull, 0xbbbull, 0x138dull, 0x1923ull, 0x1923ull, 0x138dull, 0xbbbull, 0x555ull, 0x1c7ull, 0x69ull, 0xfull, 0x1ull},
                (uint64_t[]){0x1ull, 0x10ull, 0x78ull, 0x230ull, 0x71cull, 0x1110ull, 0x1f48ull, 0x2cb0ull, 0x3246ull, 0x2cb0ull, 0x1f48ull, 0x1110ull, 0x71cull, 0x230ull, 0x78ull, 0x10ull, 0x1ull},
                (uint64_t[]){0x1ull, 0x11ull, 0x88ull, 0x2a8ull, 0x94cull, 0x182cull, 0x3058ull, 0x4bf8ull, 0x5ef6ull, 0x5ef6ull, 0x4bf8ull, 0x3058ull, 0x182cull, 0x94cull, 0x2a8ull, 0x88ull, 0x11ull, 0x1ull},
                (uint64_t[]){0x1ull, 0x12ull, 0x99ull, 0x330ull, 0xbf4ull, 0x2178ull, 0x4884ull, 0x7c50ull, 0xaaeeull, 0xbdecull, 0xaaeeull, 0x7c50ull, 0x4884ull, 0x2178ull, 0xbf4ull, 0x330ull, 0x99ull, 0x12ull, 0x1ull},
                (uint64_t[]){0x1ull, 0x13ull, 0xabull, 0x3c9ull, 0xf24ull, 0x2d6cull, 0x69fcull, 0xc4d4ull, 0x1273eull, 0x168daull, 0x168daull, 0x1273eull, 0xc4d4ull, 0x69fcull, 0x2d6cull, 0xf24ull, 0x3c9ull, 0xabull, 0x13ull, 0x1ull},
                (uint64_t[]){0x1ull, 0x14ull, 0xbeull, 0x474ull, 0x12edull, 0x3c90ull, 0x9768ull, 0x12ed0ull, 0x1ec12ull, 0x29018ull, 0x2d1b4ull, 0x29018ull, 0x1ec12ull, 0x12ed0ull, 0x9768ull, 0x3c90ull, 0x12edull, 0x474ull, 0xbeull, 0x14ull, 0x1ull},
                (uint64_t[]){0x1ull, 0x15ull, 0xd2ull, 0x532ull, 0x1761ull, 0x4f7dull, 0xd3f8ull, 0x1c638ull, 0x31ae2ull, 0x47c2aull, 0x561ccull, 0x561ccull, 0x47c2aull, 0x31ae2ull, 0x1c638ull, 0xd3f8ull, 0x4f7dull, 0x1761ull, 0x532ull, 0xd2ull, 0x15ull, 0x1ull},
                (uint64_t[]){0x1ull, 0x16ull, 0xe7ull, 0x604ull, 0x1c93ull, 0x66deull, 0x12375ull, 0x29a30ull, 0x4e11aull, 0x7970cull, 0x9ddf6ull, 0xac398ull, 0x9ddf6ull, 0x7970cull, 0x4e11aull, 0x29a30ull, 0x12375ull, 0x66deull, 0x1c93ull, 0x604ull, 0xe7ull, 0x16ull, 0x1ull},
                (uint64_t[]){0x1ull, 0x17ull, 0xfdull, 0x6ebull, 0x2297ull, 0x8371ull, 0x18a53ull, 0x3bda5ull, 0x77b4aull, 0xc7826ull, 0x117502ull, 0x14a18eull, 0x14a18eull, 0x117502ull, 0xc7826ull, 0x77b4aull, 0x3bda5ull, 0x18a53ull, 0x8371ull, 0x2297ull, 0x6ebull, 0xfdull, 0x17ull, 0x1ull},
                (uint64_t[]){0x1ull, 0x18ull, 0x114ull, 0x7e8ull, 0x2982ull, 0xa608ull, 0x20dc4ull, 0x547f8ull, 0xb38efull, 0x13f370ull, 0x1ded28ull, 0x261690ull, 0x29431cull, 0x261690ull, 0x1ded28ull, 0x13f370ull, 0xb38efull, 0x547f8ull, 0x20dc4ull, 0xa608ull, 0x2982ull, 0x7e8ull, 0x114ull, 0x18ull, 0x1ull},
                (uint64_t[]){0x1ull, 0x19ull, 0x12cull, 0x8fcull, 0x316aull, 0xcf8aull, 0x2b3ccull, 0x755bcull, 0x1080e7ull, 0x1f2c5full, 0x31e098ull, 0x4403b8ull, 0x4f59acull, 0x4f59acull, 0x4403b8ull, 0x31e098ull, 0x1f2c5full, 0x1080e7ull, 0x755bcull, 0x2b3ccull, 0xcf8aull, 0x316aull, 0x8fcull, 0x12cull, 0x19ull, 0x1ull},
                (uint64_t[]){0x1ull, 0x1aull, 0x145ull, 0xa28ull, 0x3a66ull, 0x100f4ull, 0x38356ull, 0xa0988ull, 0x17d6a3ull, 0x2fad46ull, 0x510cf7ull, 0x75e450ull, 0x935d64ull, 0x9eb358ull, 0x935d64ull, 0x75e450ull, 0x510cf7ull, 0x2fad46ull, 0x17d6a3ull, 0xa0988ull, 0x38356ull, 0x100f4ull, 0x3a66ull, 0xa28ull, 0x145ull, 0x1aull, 0x1ull},
                (uint64_t[]){0x1ull, 0x1bull, 0x15full, 0xb6dull, 0x448eull, 0x13b5aull, 0x4844aull, 0xd8cdeull, 0x21e02bull, 0x4783e9ull, 0x80ba3dull, 0xc6f147ull, 0x10941b4ull, 0x13210bcull, 0x13210bcull, 0x10941b4ull, 0xc6f147ull, 0x80ba3dull, 0x4783e9ull, 0x21e02bull, 0xd8cdeull, 0x4844aull, 0x13b5aull, 0x448eull, 0xb6dull, 0x15full, 0x1bull, 0x1ull},
                (uint64_t[]){0x1ull, 0x1cull, 0x17aull, 0xcccull, 0x4ffbull, 0x17fe8ull, 0x5bfa4ull, 0x121128ull, 0x2f6d09ull, 0x696414ull, 0xc83e26ull, 0x147ab84ull, 0x1d032fbull, 0x23b5270ull, 0x2642178ull, 0x23b5270ull, 0x1d032fbull, 0x147ab84ull, 0xc83e26ull, 0x696414ull, 0x2f6d09ull, 0x121128ull, 0x5bfa4ull, 0x17fe8ull, 0x4ffbull, 0xcccull, 0x17aull, 0x1cull, 0x1ull},
                (uint64_t[]){0x1ull, 0x1dull, 0x196ull, 0xe46ull, 0x5cc7ull, 0x1cfe3ull, 0x73f8cull, 0x17d0ccull, 0x417e31ull, 0x98d11dull, 0x131a23aull, 0x20fe9aaull, 0x317de7full, 0x40b856bull, 0x49f73e8ull, 0x49f73e8ull, 0x40b856bull, 0x317de7full, 0x20fe9aaull, 0x131a23aull, 0x98d11dull, 0x417e31ull, 0x17d0ccull, 0x73f8cull, 0x1cfe3ull, 0x5cc7ull, 0xe46ull, 0x196ull, 0x1dull, 0x1ull},
                (uint64_t[]){0x1ull, 0x1eull, 0x1b3ull, 0xfdcull, 0x6b0dull, 0x22caaull, 0x90f6full, 0x1f1058ull, 0x594efdull, 0xda4f4eull, 0x1ca7357ull, 0x3418be4ull, 0x527c829ull, 0x72363eaull, 0x8aaf953ull, 0x93ee7d0ull, 0x8aaf953ull, 0x72363eaull, 0x527c829ull, 0x3418be4ull, 0x1ca7357ull, 0xda4f4eull, 0x594efdull, 0x1f1058ull, 0x90f6full, 0x22caaull, 0x6b0dull, 0xfdcull, 0x1b3ull, 0x1eull, 0x1ull},
                (uint64_t[]){0x1ull, 0x1full, 0x1d1ull, 0x118full, 0x7ae9ull, 0x297b7ull, 0xb3c19ull, 0x281fc7ull, 0x785f55ull, 0x1339e4bull, 0x2a4c2a5ull, 0x50bff3bull, 0x869540dull, 0xc4b2c13ull, 0xfce5d3dull, 0x11e9e123ull, 0x11e9e123ull, 0xfce5d3dull, 0xc4b2c13ull, 0x869540dull, 0x50bff3bull, 0x2a4c2a5ull, 0x1339e4bull, 0x785f55ull, 0x281fc7ull, 0xb3c19ull, 0x297b7ull, 0x7ae9ull, 0x118full, 0x1d1ull, 0x1full, 0x1ull},
                (uint64_t[]){0x1ull, 0x20ull, 0x1f0ull, 0x1360ull, 0x8c78ull, 0x312a0ull, 0xdd3d0ull, 0x335be0ull, 0xa07f1cull, 0x1abfda0ull, 0x3d860f0ull, 0x7b0c1e0ull, 0xd755348ull, 0x14b48020ull, 0x1c198950ull, 0x21b83e60ull, 0x23d3c246ull, 0x21b83e60ull, 0x1c198950ull, 0x14b48020ull, 0xd755348ull, 0x7b0c1e0ull, 0x3d860f0ull, 0x1abfda0ull, 0xa07f1cull, 0x335be0ull, 0xdd3d0ull, 0x312a0ull, 0x8c78ull, 0x1360ull, 0x1f0ull, 0x20ull, 0x1ull},
                (uint64_t[]){0x1ull, 0x21ull, 0x210ull, 0x1550ull, 0x9fd8ull, 0x39f18ull, 0x10e670ull, 0x412fb0ull, 0xd3dafcull, 0x24c7cbcull, 0x5845e90ull, 0xb8922d0ull, 0x15261528ull, 0x2229d368ull, 0x30ce0970ull, 0x3dd1c7b0ull, 0x458c00a6ull, 0x458c00a6ull, 0x3dd1c7b0ull, 0x30ce0970ull, 0x2229d368ull, 0x15261528ull, 0xb8922d0ull, 0x5845e90ull, 0x24c7cbcull, 0xd3dafcull, 0x412fb0ull, 0x10e670ull, 0x39f18ull, 0x9fd8ull, 0x1550ull, 0x210ull, 0x21ull, 0x1ull},
                (uint64_t[]){0x1ull, 0x22ull, 0x231ull, 0x1760ull, 0xb528ull, 0x43ef0ull, 0x148588ull, 0x521620ull, 0x1150aacull, 0x32057b8ull, 0x7d0db4cull, 0x110d8160ull, 0x20af37f8ull, 0x374fe890ull, 0x52f7dcd8ull, 0x6e9fd120ull, 0x835dc856ull, 0x8b18014cull, 0x835dc856ull, 0x6e9fd120ull, 0x52f7dcd8ull, 0x374fe890ull, 0x20af37f8ull, 0x110d8160ull, 0x7d0db4cull, 0x32057b8ull, 0x1150aacull, 0x521620ull, 0x148588ull, 0x43ef0ull, 0xb528ull, 0x1760ull, 0x231ull, 0x22ull, 0x1ull},
                (uint64_t[]){0x1ull, 0x23ull, 0x253ull, 0x1991ull, 0xcc88ull, 0x4f418ull, 0x18c478ull, 0x669ba8ull, 0x16720ccull, 0x4356264ull, 0xaf13304ull, 0x18de5cacull, 0x31bcb958ull, 0x57ff2088ull, 0x8a47c568ull, 0xc197adf8ull, 0xf1fd9976ull, 0x10e75c9a2ull, 0x10e75c9a2ull, 0xf1fd9976ull, 0xc197adf8ull, 0x8a47c568ull, 0x57ff2088ull, 0x31bcb958ull, 0x18de5cacull, 0xaf13304ull, 0x4356264ull, 0x16720ccull, 0x669ba8ull, 0x18c478ull, 0x4f418ull, 0xcc88ull, 0x1991ull, 0x253ull, 0x23ull, 0x1ull},
                (uint64_t[]){0x1ull, 0x24ull, 0x276ull, 0x1be4ull, 0xe619ull, 0x5c0a0ull, 0x1db890ull, 0x7f6020ull, 0x1cdbc74ull, 0x59c8330ull, 0xf269568ull, 0x23cf8fb0ull, 0x4a9b1604ull, 0x89bbd9e0ull, 0xe246e5f0ull, 0x14bdf7360ull, 0x1b395476eull, 0x200736318ull, 0x21ceb9344ull, 0x200736318ull, 0x1b395476eull, 0x14bdf7360ull, 0xe246e5f0ull, 0x89bbd9e0ull, 0x4a9b1604ull, 0x23cf8fb0ull, 0xf269568ull, 0x59c8330ull, 0x1cdbc74ull, 0x7f6020ull, 0x1db890ull, 0x5c0a0ull, 0xe619ull, 0x1be4ull, 0x276ull, 0x24ull, 0x1ull},
                (uint64_t[]){0x1ull, 0x25ull, 0x29aull, 0x1e5aull, 0x101fdull, 0x6a6b9ull, 0x237930ull, 0x9d18b0ull, 0x24d1c94ull, 0x76a3fa4ull, 0x14c31898ull, 0x32f62518ull, 0x6e6aa5b4ull, 0xd456efe4ull, 0x16c02bfd0ull, 0x22e265950ull, 0x2ff74baceull, 0x3b408aa86ull, 0x41d5ef65cull, 0x41d5ef65cull, 0x3b408aa86ull, 0x2ff74baceull, 0x22e265950ull, 0x16c02bfd0ull, 0xd456efe4ull, 0x6e6aa5b4ull, 0x32f62518ull, 0x14c31898ull, 0x76a3fa4ull, 0x24d1c94ull, 0x9d18b0ull, 0x237930ull, 0x6a6b9ull, 0x101fdull, 0x1e5aull, 0x29aull, 0x25ull, 0x1ull},
                (uint64_t[]){0x1ull, 0x26ull, 0x2bfull, 0x20f4ull, 0x12057ull, 0x7a8b6ull, 0x2a1fe9ull, 0xc091e0ull, 0x2ea3544ull, 0x9b75c38ull, 0x1c2d583cull, 0x47b93db0ull, 0xa160caccull, 0x142c19598ull, 0x24059afb4ull, 0x39a291920ull, 0x52d9b141eull, 0x6b37d6554ull, 0x7d167a0e2ull, 0x83abdecb8ull, 0x7d167a0e2ull, 0x6b37d6554ull, 0x52d9b141eull, 0x39a291920ull, 0x24059afb4ull, 0x142c19598ull, 0xa160caccull, 0x47b93db0ull, 0x1c2d583cull, 0x9b75c38ull, 0x2ea3544ull, 0xc091e0ull, 0x2a1fe9ull, 0x7a8b6ull, 0x12057ull, 0x20f4ull, 0x2bfull, 0x26ull, 0x1ull},
                (uint64_t[]){0x1ull, 0x27ull, 0x2e5ull, 0x23b3ull, 0x1414bull, 0x8c90dull, 0x31c89full, 0xeab1c9ull, 0x3aac724ull, 0xca1917cull, 0x25e4b474ull, 0x63e695ecull, 0xe91a087cull, 0x1e4226064ull, 0x3831b454cull, 0x5da82c8d4ull, 0x8c7c42d3eull, 0xbe1187972ull, 0xe84e50636ull, 0x100c258d9aull, 0x100c258d9aull, 0xe84e50636ull, 0xbe1187972ull, 0x8c7c42d3eull, 0x5da82c8d4ull, 0x3831b454cull, 0x1e4226064ull, 0xe91a087cull, 0x63e695ecull, 0x25e4b474ull, 0xca1917cull, 0x3aac724ull, 0xeab1c9ull, 0x31c89full, 0x8c90dull, 0x1414bull, 0x23b3ull, 0x2e5ull, 0x27ull, 0x1ull},
                (uint64_t[]){0x1ull, 0x28ull, 0x30cull, 0x2698ull, 0x164feull, 0xa0a58ull, 0x3a91acull, 0x11c7a68ull, 0x49578edull, 0x104c58a0ull, 0x328645f0ull, 0x89cb4a60ull, 0x14d009e68ull, 0x2cd3c68e0ull, 0x5673da5b0ull, 0x95d9e0e20ull, 0xea246f612ull, 0x14a8dca6b0ull, 0x1a65fd7fa8ull, 0x1e910a93d0ull, 0x20184b1b34ull, 0x1e910a93d0ull, 0x1a65fd7fa8ull, 0x14a8dca6b0ull, 0xea246f612ull, 0x95d9e0e20ull, 0x5673da5b0ull, 0x2cd3c68e0ull, 0x14d009e68ull, 0x89cb4a60ull, 0x328645f0ull, 0x104c58a0ull, 0x49578edull, 0x11c7a68ull, 0x3a91acull, 0xa0a58ull, 0x164feull, 0x2698ull, 0x30cull, 0x28ull, 0x1ull},
                (uint64_t[]){0x1ull, 0x29ull, 0x334ull, 0x29a4ull, 0x18b96ull, 0xb6f56ull, 0x449c04ull, 0x1570c14ull, 0x5b1f355ull, 0x14e1d18dull, 0x42d29e90ull, 0xbc519050ull, 0x1d6cbe8c8ull, 0x41a3d0748ull, 0x8347a0e90ull, 0xec4dbb3d0ull, 0x17ffe50432ull, 0x234b239cc2ull, 0x2f0eda2658ull, 0x38f7081378ull, 0x3ea955af04ull, 0x3ea955af04ull, 0x38f7081378ull, 0x2f0eda2658ull, 0x234b239cc2ull, 0x17ffe50432ull, 0xec4dbb3d0ull, 0x8347a0e90ull, 0x41a3d0748ull, 0x1d6cbe8c8ull, 0xbc519050ull, 0x42d29e90ull, 0x14e1d18dull, 0x5b1f355ull, 0x1570c14ull, 0x449c04ull, 0xb6f56ull, 0x18b96ull, 0x29a4ull, 0x334ull, 0x29ull, 0x1ull},
                (uint64_t[]){0x1ull, 0x2aull, 0x35dull, 0x2cd8ull, 0x1b53aull, 0xcfaecull, 0x500b5aull, 0x19ba818ull, 0x708ff69ull, 0x1a93c4e2ull, 0x57b4701dull, 0xff242ee0ull, 0x2931d7918ull, 0x5f108f010ull, 0xc4eb715d8ull, 0x16f955c260ull, 0x26c4c0b802ull, 0x3b4b08a0f4ull, 0x5259fdc31aull, 0x6805e239d0ull, 0x77a05dc27cull, 0x7d52ab5e08ull, 0x77a05dc27cull, 0x6805e239d0ull, 0x5259fdc31aull, 0x3b4b08a0f4ull, 0x26c4c0b802ull, 0x16f955c260ull, 0xc4eb715d8ull, 0x5f108f010ull, 0x2931d7918ull, 0xff242ee0ull, 0x57b4701dull, 0x1a93c4e2ull, 0x708ff69ull, 0x19ba818ull, 0x500b5aull, 0xcfaecull, 0x1b53aull, 0x2cd8ull, 0x35dull, 0x2aull, 0x1ull},
                (uint64_t[]){0x1ull, 0x2bull, 0x387ull, 0x3035ull, 0x1e212ull, 0xeb026ull, 0x5d0646ull, 0x1ebb372ull, 0x8a4a781ull, 0x219cc44bull, 0x724834ffull, 0x156d89efdull, 0x39241a7f8ull, 0x884266928ull, 0x123fc005e8ull, 0x23480cd838ull, 0x3dbe167a62ull, 0x620fc958f6ull, 0x8da506640eull, 0xba5fdffceaull, 0xdfa63ffc4cull, 0xf4f3092084ull, 0xf4f3092084ull, 0xdfa63ffc4cull, 0xba5fdffceaull, 0x8da506640eull, 0x620fc958f6ull, 0x3dbe167a62ull, 0x23480cd838ull, 0x123fc005e8ull, 0x884266928ull, 0x39241a7f8ull, 0x156d89efdull, 0x724834ffull, 0x219cc44bull, 0x8a4a781ull, 0x1ebb372ull, 0x5d0646ull, 0xeb026ull, 0x1e212ull, 0x3035ull, 0x387ull, 0x2bull, 0x1ull},
                (uint64_t[]){0x1ull, 0x2cull, 0x3b2ull, 0x33bcull, 0x21247ull, 0x109238ull, 0x6bb66cull, 0x248b9b8ull, 0xa905af3ull, 0x2a416bccull, 0x93e4f94aull, 0x1c920d3fcull, 0x4e91a46f5ull, 0xc16681120ull, 0x1ac3e66f10ull, 0x3587ccde20ull, 0x610623529aull, 0x9fcddfd358ull, 0xefb4cfbd04ull, 0x14804e660f8ull, 0x19a061ff936ull, 0x1d499491cd0ull, 0x1e9e6124108ull, 0x1d499491cd0ull, 0x19a061ff936ull, 0x14804e660f8ull, 0xefb4cfbd04ull, 0x9fcddfd358ull, 0x610623529aull, 0x3587ccde20ull, 0x1ac3e66f10ull, 0xc16681120ull, 0x4e91a46f5ull, 0x1c920d3fcull, 0x93e4f94aull, 0x2a416bccull, 0xa905af3ull, 0x248b9b8ull, 0x6bb66cull, 0x109238ull, 0x21247ull, 0x33bcull, 0x3b2ull, 0x2cull, 0x1ull},
                (uint64_t[]){0x1ull, 0x2dull, 0x3deull, 0x376eull, 0x24603ull, 0x12a47full, 0x7c48a4ull, 0x2b47024ull, 0xcd914abull, 0x34d1c6bfull, 0xbe266516ull, 0x25d05cd46ull, 0x6b23b1af1ull, 0x10ff825815ull, 0x26da4e8030ull, 0x504bb34d30ull, 0x968df030baull, 0x100d40325f2ull, 0x18f82af905cull, 0x237b9b61dfcull, 0x2e20b065a2eull, 0x36e9f691606ull, 0x3be7f5b5dd8ull, 0x3be7f5b5dd8ull, 0x36e9f691606ull, 0x2e20b065a2eull, 0x237b9b61dfcull, 0x18f82af905cull, 0x100d40325f2ull, 0x968df030baull, 0x504bb34d30ull, 0x26da4e8030ull, 0x10ff825815ull, 0x6b23b1af1ull, 0x25d05cd46ull, 0xbe266516ull, 0x34d1c6bfull, 0xcd914abull, 0x2b47024ull, 0x7c48a4ull, 0x12a47full, 0x24603ull, 0x376eull, 0x3deull, 0x2dull, 0x1ull},
                (uint64_t[]){0x1ull, 0x2eull, 0x40bull, 0x3b4cull, 0x27d71ull, 0x14ea82ull, 0x8eed23ull, 0x330b8c8ull, 0xf8d84cfull, 0x41aadb6aull, 0xf2f82bd5ull, 0x31b2c325cull, 0x90f40e837ull, 0x17b1bd7306ull, 0x37d9d0d845ull, 0x772601cd60ull, 0xe6d9a37deaull, 0x19761f356acull, 0x29056b2b64eull, 0x3c73c65ae58ull, 0x519c4bc782aull, 0x650aa6f7034ull, 0x72d1ec473deull, 0x77cfeb6bbb0ull, 0x72d1ec473deull, 0x650aa6f7034ull, 0x519c4bc782aull, 0x3c73c65ae58ull, 0x29056b2b64eull, 0x19761f356acull, 0xe6d9a37deaull, 0x772601cd60ull, 0x37d9d0d845ull, 0x17b1bd7306ull, 0x90f40e837ull, 0x31b2c325cull, 0xf2f82bd5ull, 0x41aadb6aull, 0xf8d84cfull, 0x330b8c8ull, 0x8eed23ull, 0x14ea82ull, 0x27d71ull, 0x3b4cull, 0x40bull, 0x2eull, 0x1ull},
                (uint64_t[]){0x1ull, 0x2full, 0x439ull, 0x3f57ull, 0x2b8bdull, 0x1767f3ull, 0xa3d7a5ull, 0x3bfa5ebull, 0x12be3d97ull, 0x51386039ull, 0x134a3073full, 0x40e245e31ull, 0xc2a6d1a93ull, 0x20c0fe5b3dull, 0x4f8b8e4b4bull, 0xaeffd2a5a5ull, 0x15dffa54b4aull, 0x27e3b96d496ull, 0x427b8a60cfaull, 0x657931864a6ull, 0x8e101222682ull, 0xb6a6f2be85eull, 0xd7dc933e412ull, 0xeaa1d7b2f8eull, 0xeaa1d7b2f8eull, 0xd7dc933e412ull, 0xb6a6f2be85eull, 0x8e101222682ull, 0x657931864a6ull, 0x427b8a60cfaull, 0x27e3b96d496ull, 0x15dffa54b4aull, 0xaeffd2a5a5ull, 0x4f8b8e4b4bull, 0x20c0fe5b3dull, 0xc2a6d1a93ull, 0x40e245e31ull, 0x134a3073full, 0x51386039ull, 0x12be3d97ull, 0x3bfa5ebull, 0xa3d7a5ull, 0x1767f3ull, 0x2b8bdull, 0x3f57ull, 0x439ull, 0x2full, 0x1ull},
                (uint64_t[]){0x1ull, 0x30ull, 0x468ull, 0x4390ull, 0x2f814ull, 0x1a20b0ull, 0xbb3f98ull, 0x4637d90ull, 0x167de382ull, 0x63f69dd0ull, 0x185db6778ull, 0x542c76570ull, 0x10389178c4ull, 0x2ceb6b75d0ull, 0x704c8ca688ull, 0xfe8b60f0f0ull, 0x20cff77f0efull, 0x3dc3b3c1fe0ull, 0x6a5f43ce190ull, 0xa7f4bbe71a0ull, 0xf38943a8b28ull, 0x144b704e0ee0ull, 0x18e8385fcc70ull, 0x1c27e6af13a0ull, 0x1d543af65f1cull, 0x1c27e6af13a0ull, 0x18e8385fcc70ull, 0x144b704e0ee0ull, 0xf38943a8b28ull, 0xa7f4bbe71a0ull, 0x6a5f43ce190ull, 0x3dc3b3c1fe0ull, 0x20cff77f0efull, 0xfe8b60f0f0ull, 0x704c8ca688ull, 0x2ceb6b75d0ull, 0x10389178c4ull, 0x542c76570ull, 0x185db6778ull, 0x63f69dd0ull, 0x167de382ull, 0x4637d90ull, 0xbb3f98ull, 0x1a20b0ull, 0x2f814ull, 0x4390ull, 0x468ull, 0x30ull, 0x1ull},
                (uint64_t[]){0x1ull, 0x31ull, 0x498ull, 0x47f8ull, 0x33ba4ull, 0x1d18c4ull, 0xd56048ull, 0x51ebd28ull, 0x1ae16112ull, 0x7a748152ull, 0x1e9d20548ull, 0x6c8a2cce8ull, 0x157b58de34ull, 0x3d23fcee94ull, 0x9d37f81c58ull, 0x16ed7ed9778ull, 0x30b8ad8e1dfull, 0x5e93ab410cfull, 0xa822f790170ull, 0x11253ffb5330ull, 0x19b7dff8fcc8ull, 0x238404889a08ull, 0x2d33a8addb50ull, 0x35101f0ee010ull, 0x397c21a572bcull, 0x397c21a572bcull, 0x35101f0ee010ull, 0x2d33a8addb50ull, 0x238404889a08ull, 0x19b7dff8fcc8ull, 0x11253ffb5330ull, 0xa822f790170ull, 0x5e93ab410cfull, 0x30b8ad8e1dfull, 0x16ed7ed9778ull, 0x9d37f81c58ull, 0x3d23fcee94ull, 0x157b58de34ull, 0x6c8a2cce8ull, 0x1e9d20548ull, 0x7a748152ull, 0x1ae16112ull, 0x51ebd28ull, 0xd56048ull, 0x1d18c4ull, 0x33ba4ull, 0x47f8ull, 0x498ull, 0x31ull, 0x1ull},
                (uint64_t[]){0x1ull, 0x32ull, 0x4c9ull, 0x4c90ull, 0x3839cull, 0x205468ull, 0xf2790cull, 0x5f41d70ull, 0x20001e3aull, 0x9555e264ull, 0x26446869aull, 0x8b274d230ull, 0x1c43fbab1cull, 0x529f55ccc8ull, 0xda5bf50aecull, 0x20c0fe5b3d0ull, 0x47a62c67957ull, 0x8f4c58cf2aeull, 0x106b6a2d123full, 0x1ba76f7454a0ull, 0x2add1ff44ff8ull, 0x3d3be48196d0ull, 0x50b7ad367558ull, 0x6243c7bcbb60ull, 0x6e8c40b452ccull, 0x72f8434ae578ull, 0x6e8c40b452ccull, 0x6243c7bcbb60ull, 0x50b7ad367558ull, 0x3d3be48196d0ull, 0x2add1ff44ff8ull, 0x1ba76f7454a0ull, 0x106b6a2d123full, 0x8f4c58cf2aeull, 0x47a62c67957ull, 0x20c0fe5b3d0ull, 0xda5bf50aecull, 0x529f55ccc8ull, 0x1c43fbab1cull, 0x8b274d230ull, 0x26446869aull, 0x9555e264ull, 0x20001e3aull, 0x5f41d70ull, 0xf2790cull, 0x205468ull, 0x3839cull, 0x4c90ull, 0x4c9ull, 0x32ull, 0x1ull},
                (uint64_t[]){0x1ull, 0x33ull, 0x4fbull, 0x5159ull, 0x3d02cull, 0x23d804ull, 0x112cd74ull, 0x6e6967cull, 0x25f43baaull, 0xb556009eull, 0x2f99c68feull, 0xb16bb58caull, 0x24f6707d4cull, 0x6ee35177e4ull, 0x12cfb4ad7b4ull, 0x2e66bdabebcull, 0x68672ac2d27ull, 0xd6f28536c05ull, 0x19602fba04edull, 0x2c12d9a166dfull, 0x46848f68a498ull, 0x68190475e6c8ull, 0x8df391b80c28ull, 0xb2fb74f330b8ull, 0xd0d008710e2cull, 0xe18483ff3844ull, 0xe18483ff3844ull, 0xd0d008710e2cull, 0xb2fb74f330b8ull, 0x8df391b80c28ull, 0x68190475e6c8ull, 0x46848f68a498ull, 0x2c12d9a166dfull, 0x19602fba04edull, 0xd6f28536c05ull, 0x68672ac2d27ull, 0x2e66bdabebcull, 0x12cfb4ad7b4ull, 0x6ee35177e4ull, 0x24f6707d4cull, 0xb16bb58caull, 0x2f99c68feull, 0xb556009eull, 0x25f43baaull, 0x6e6967cull, 0x112cd74ull, 0x23d804ull, 0x3d02cull, 0x5159ull, 0x4fbull, 0x33ull, 0x1ull},
                (uint64_t[]){0x1ull, 0x34ull, 0x52eull, 0x5654ull, 0x42185ull, 0x27a830ull, 0x136a578ull, 0x7f963f0ull, 0x2cdad226ull, 0xdb4a3c48ull, 0x3aef2699cull, 0xe1057c1c8ull, 0x300d2bd616ull, 0x93d9c1f530ull, 0x19bde9c4f98ull, 0x41367259670ull, 0x96cde86ebe3ull, 0x13f59aff992cull, 0x26cf580d70f2ull, 0x4573095b6bccull, 0x7297690a0b77ull, 0xae9d93de8b60ull, 0xf60c962df2f0ull, 0x140ef06ab3ce0ull, 0x183cb7d643ee4ull, 0x1b2548c704670ull, 0x1c30907fe7088ull, 0x1b2548c704670ull, 0x183cb7d643ee4ull, 0x140ef06ab3ce0ull, 0xf60c962df2f0ull, 0xae9d93de8b60ull, 0x7297690a0b77ull, 0x4573095b6bccull, 0x26cf580d70f2ull, 0x13f59aff992cull, 0x96cde86ebe3ull, 0x41367259670ull, 0x19bde9c4f98ull, 0x93d9c1f530ull, 0x300d2bd616ull, 0xe1057c1c8ull, 0x3aef2699cull, 0xdb4a3c48ull, 0x2cdad226ull, 0x7f963f0ull, 0x136a578ull, 0x27a830ull, 0x42185ull, 0x5654ull, 0x52eull, 0x34ull, 0x1ull},
                (uint64_t[]){0x1ull, 0x35ull, 0x562ull, 0x5b82ull, 0x477d9ull, 0x2bc9b5ull, 0x15e4da8ull, 0x9300968ull, 0x34d43616ull, 0x108250e6eull, 0x48a3ca5e4ull, 0x11bf4a2b64ull, 0x3e1d8397deull, 0xc3e6edcb46ull, 0x22fb85e44c8ull, 0x5af45c1e608ull, 0xd8045ac8253ull, 0x1d627986850full, 0x3ac4f30d0a1eull, 0x6c426168dcbeull, 0xb80a72657743ull, 0x12134fce896d7ull, 0x1a4aa2a0c7e50ull, 0x236fb9cd92fd0ull, 0x2c4ba840f7bc4ull, 0x3362009d48554ull, 0x3755d946eb6f8ull, 0x3755d946eb6f8ull, 0x3362009d48554ull, 0x2c4ba840f7bc4ull, 0x236fb9cd92fd0ull, 0x1a4aa2a0c7e50ull, 0x12134fce896d7ull, 0xb80a72657743ull, 0x6c426168dcbeull, 0x3ac4f30d0a1eull, 0x1d627986850full, 0xd8045ac8253ull, 0x5af45c1e608ull, 0x22fb85e44c8ull, 0xc3e6edcb46ull, 0x3e1d8397deull, 0x11bf4a2b64ull, 0x48a3ca5e4ull, 0x108250e6eull, 0x34d43616ull, 0x9300968ull, 0x15e4da8ull, 0x2bc9b5ull, 0x477d9ull, 0x5b82ull, 0x562ull, 0x35ull, 0x1ull},
                (uint64_t[]){0x1ull, 0x36ull, 0x597ull, 0x60e4ull, 0x4d35bull, 0x30418eull, 0x18a175dull, 0xa8e5710ull, 0x3e043f7eull, 0x13cf94484ull, 0x59261b452ull, 0x164986d148ull, 0x4fdccdc342ull, 0x10204716324ull, 0x2f39f4c100eull, 0x7defe202ad0ull, 0x132f8b6e685bull, 0x2ae2bf330762ull, 0x58276c938f2dull, 0xa7075475e6dcull, 0x1244cd3ce5401ull, 0x1d93f6f4e0e1aull, 0x2c5df26f51527ull, 0x3dba5c6e5ae20ull, 0x4fbb620e8ab94ull, 0x5fada8de40118ull, 0x6ab7d9e433c4cull, 0x6eabb28dd6df0ull, 0x6ab7d9e433c4cull, 0x5fada8de40118ull, 0x4fbb620e8ab94ull, 0x3dba5c6e5ae20ull, 0x2c5df26f51527ull, 0x1d93f6f4e0e1aull, 0x1244cd3ce5401ull, 0xa7075475e6dcull, 0x58276c938f2dull, 0x2ae2bf330762ull, 0x132f8b6e685bull, 0x7defe202ad0ull, 0x2f39f4c100eull, 0x10204716324ull, 0x4fdccdc342ull, 0x164986d148ull, 0x59261b452ull, 0x13cf94484ull, 0x3e043f7eull, 0xa8e5710ull, 0x18a175dull, 0x30418eull, 0x4d35bull, 0x60e4ull, 0x597ull, 0x36ull, 0x1ull},
                (uint64_t[]){0x1ull, 0x37ull, 0x5cdull, 0x667bull, 0x5343full, 0x3514e9ull, 0x1ba58ebull, 0xc186e6dull, 0x4892968eull, 0x17afd8402ull, 0x6cf5af8d6ull, 0x1bdbe8859aull, 0x662654948aull, 0x151e13f2666ull, 0x3f5a3bd7332ull, 0xad29d6c3adeull, 0x1b0e898e932bull, 0x3e124aa16fbdull, 0x830a2bc6968full, 0xff2ec1097609ull, 0x1cb5428443addull, 0x2fd8c431c621bull, 0x49f1e96432341ull, 0x6a184eddac347ull, 0x8d75be7ce59b4ull, 0xaf690aeccacacull, 0xca6582c273d64ull, 0xd9638c720aa3cull, 0xd9638c720aa3cull, 0xca6582c273d64ull, 0xaf690aeccacacull, 0x8d75be7ce59b4ull, 0x6a184eddac347ull, 0x49f1e96432341ull, 0x2fd8c431c621bull, 0x1cb5428443addull, 0xff2ec1097609ull, 0x830a2bc6968full, 0x3e124aa16fbdull, 0x1b0e898e932bull, 0xad29d6c3adeull, 0x3f5a3bd7332ull, 0x151e13f2666ull, 0x662654948aull, 0x1bdbe8859aull, 0x6cf5af8d6ull, 0x17afd8402ull, 0x4892968eull, 0xc186e6dull, 0x1ba58ebull, 0x3514e9ull, 0x5343full, 0x667bull, 0x5cdull, 0x37ull, 0x1ull},
                (uint64_t[]){0x1ull, 0x38ull, 0x604ull, 0x6c48ull, 0x59abaull, 0x3a4928ull, 0x1ef6dd4ull, 0xdd2c758ull, 0x54ab04fbull, 0x1c3901a90ull, 0x84a587cd8ull, 0x22ab437e70ull, 0x82023d1a24ull, 0x1b80793baf0ull, 0x54784fc9998ull, 0xec84129ae10ull, 0x25e126face09ull, 0x5920d43002e8ull, 0xc11c7668064cull, 0x18238ecd00c98ull, 0x2ca82e94db0e6ull, 0x4c8e06b609cf8ull, 0x79caad95f855cull, 0xb40a3841de688ull, 0xf78e0d5a91cfbull, 0x13cdec969b0660ull, 0x179ce8daf3ea10ull, 0x1a3c90f347e7a0ull, 0x1b2c718e415478ull, 0x1a3c90f347e7a0ull, 0x179ce8daf3ea10ull, 0x13cdec969b0660ull, 0xf78e0d5a91cfbull, 0xb40a3841de688ull, 0x79caad95f855cull, 0x4c8e06b609cf8ull, 0x2ca82e94db0e6ull, 0x18238ecd00c98ull, 0xc11c7668064cull, 0x5920d43002e8ull, 0x25e126face09ull, 0xec84129ae10ull, 0x54784fc9998ull, 0x1b80793baf0ull, 0x82023d1a24ull, 0x22ab437e70ull, 0x84a587cd8ull, 0x1c3901a90ull, 0x54ab04fbull, 0xdd2c758ull, 0x1ef6dd4ull, 0x3a4928ull, 0x59abaull, 0x6c48ull, 0x604ull, 0x38ull, 0x1ull},
                (uint64_t[]){0x1ull, 0x39ull, 0x63cull, 0x724cull, 0x60702ull, 0x3fe3e2ull, 0x229b6fcull, 0xfc2352cull, 0x627dcc53ull, 0x2183b1f8bull, 0xa0de89768ull, 0x2af59bfb48ull, 0xa4ad809894ull, 0x23a09d0d514ull, 0x6ff8c905488ull, 0x140fc62647a8ull, 0x34a968247c19ull, 0x7f01fb2ad0f1ull, 0x11a3d4a980934ull, 0x24355633812e4ull, 0x44cbbd61dbd7eull, 0x7936354ae4ddeull, 0xc658b44c02254ull, 0x12dd4e5d7d6be4ull, 0x1ab98459c70383ull, 0x2346cd6c44235bull, 0x2b6ad5718ef070ull, 0x31d979ce3bd1b0ull, 0x35690281893c18ull, 0x35690281893c18ull, 0x31d979ce3bd1b0ull, 0x2b6ad5718ef070ull, 0x2346cd6c44235bull, 0x1ab98459c70383ull, 0x12dd4e5d7d6be4ull, 0xc658b44c02254ull, 0x7936354ae4ddeull, 0x44cbbd61dbd7eull, 0x24355633812e4ull, 0x11a3d4a980934ull, 0x7f01fb2ad0f1ull, 0x34a968247c19ull, 0x140fc62647a8ull, 0x6ff8c905488ull, 0x23a09d0d514ull, 0xa4ad809894ull, 0x2af59bfb48ull, 0xa0de89768ull, 0x2183b1f8bull, 0x627dcc53ull, 0xfc2352cull, 0x229b6fcull, 0x3fe3e2ull, 0x60702ull, 0x724cull, 0x63cull, 0x39ull, 0x1ull},
                (uint64_t[]){0x1ull, 0x3aull, 0x675ull, 0x7888ull, 0x6794eull, 0x45eae4ull, 0x2699adeull, 0x11ebec28ull, 0x7240017full, 0x27ab8ebdeull, 0xc2623b6f3ull, 0x35038492b0ull, 0xcfa31c93dcull, 0x2deb7516da8ull, 0x9399661299cull, 0x1b0f52b69c30ull, 0x48b92e4ac3c1ull, 0xb3ab634f4d0aull, 0x1993f45c2da25ull, 0x35d92add01c18ull, 0x690113955d062ull, 0xbe01f2acc0b5cull, 0x13f8ee996e7032ull, 0x1f42d9a23d8e38ull, 0x2d96d2b7446f67ull, 0x3e0051c60b26deull, 0x4eb1a2ddd313cbull, 0x5d444f3fcac220ull, 0x67427c4fc50dc8ull, 0x6ad20503127830ull, 0x67427c4fc50dc8ull, 0x5d444f3fcac220ull, 0x4eb1a2ddd313cbull, 0x3e0051c60b26deull, 0x2d96d2b7446f67ull, 0x1f42d9a23d8e38ull, 0x13f8ee996e7032ull, 0xbe01f2acc0b5cull, 0x690113955d062ull, 0x35d92add01c18ull, 0x1993f45c2da25ull, 0xb3ab634f4d0aull, 0x48b92e4ac3c1ull, 0x1b0f52b69c30ull, 0x9399661299cull, 0x2deb7516da8ull, 0xcfa31c93dcull, 0x35038492b0ull, 0xc2623b6f3ull, 0x27ab8ebdeull, 0x7240017full, 0x11ebec28ull, 0x2699adeull, 0x45eae4ull, 0x6794eull, 0x7888ull, 0x675ull, 0x3aull, 0x1ull},
                (uint64_t[]){0x1ull, 0x3bull, 0x6afull, 0x7efdull, 0x6f1d6ull, 0x4c6432ull, 0x2af85c2ull, 0x14558706ull, 0x842beda7ull, 0x2ecf8ed5dull, 0xea0dca2d1ull, 0x4129a849a3ull, 0x104a6a1268cull, 0x3ae5a6e0184ull, 0xc184db29744ull, 0x2448e917c5ccull, 0x63c881015ff1ull, 0xfc64919a10cbull, 0x24ceaa912272full, 0x4f6d1f392f63dull, 0x9eda3e725ec7aull, 0x1270306421dbbeull, 0x1fd90dc43a7b8eull, 0x333bc83babfe6aull, 0x4cd9ac5981fd9full, 0x6b97247d4f9645ull, 0x8cb1f4a3de3aa9ull, 0xabf5f21d9dd5ebull, 0xc486cb8f8fcfe8ull, 0xd2148152d785f8ull, 0xd2148152d785f8ull, 0xc486cb8f8fcfe8ull, 0xabf5f21d9dd5ebull, 0x8cb1f4a3de3aa9ull, 0x6b97247d4f9645ull, 0x4cd9ac5981fd9full, 0x333bc83babfe6aull, 0x1fd90dc43a7b8eull, 0x1270306421dbbeull, 0x9eda3e725ec7aull, 0x4f6d1f392f63dull, 0x24ceaa912272full, 0xfc64919a10cbull, 0x63c881015ff1ull, 0x2448e917c5ccull, 0xc184db29744ull, 0x3ae5a6e0184ull, 0x104a6a1268cull, 0x4129a849a3ull, 0xea0dca2d1ull, 0x2ecf8ed5dull, 0x842beda7ull, 0x14558706ull, 0x2af85c2ull, 0x4c6432ull, 0x6f1d6ull, 0x7efdull, 0x6afull, 0x3bull, 0x1ull},
                (uint64_t[]){0x1ull, 0x3cull, 0x6eaull, 0x85acull, 0x770d3ull, 0x535608ull, 0x2fbe9f4ull, 0x17050cc8ull, 0x988174adull, 0x37124db04ull, 0x118dd5902eull, 0x4fca84ec74ull, 0x145d049702full, 0x4b3010f2810ull, 0xfc6a82098c8ull, 0x306136ca5d10ull, 0x88116a1925bdull, 0x1602d129b70bcull, 0x3494f3aac37faull, 0x743bc9ca51d6cull, 0xee475dab8e2b7ull, 0x1c5dd44b47c838ull, 0x32493e285c574cull, 0x5314d5ffe679f8ull, 0x801574952dfc09ull, 0xb870d0d6d193e4ull, 0xf84919212dd0eeull, 0x138a7e6c17c1094ull, 0x1707cbdad2da5d3ull, 0x1969b4ce26755e0ull, 0x1a42902a5af0bf0ull, 0x1969b4ce26755e0ull, 0x1707cbdad2da5d3ull, 0x138a7e6c17c1094ull, 0xf84919212dd0eeull, 0xb870d0d6d193e4ull, 0x801574952dfc09ull, 0x5314d5ffe679f8ull, 0x32493e285c574cull, 0x1c5dd44b47c838ull, 0xee475dab8e2b7ull, 0x743bc9ca51d6cull, 0x3494f3aac37faull, 0x1602d129b70bcull, 0x88116a1925bdull, 0x306136ca5d10ull, 0xfc6a82098c8ull, 0x4b3010f2810ull, 0x145d049702full, 0x4fca84ec74ull, 0x118dd5902eull, 0x37124db04ull, 0x988174adull, 0x17050cc8ull, 0x2fbe9f4ull, 0x535608ull, 0x770d3ull, 0x85acull, 0x6eaull, 0x3cull, 0x1ull},
                (uint64_t[]){0x1ull, 0x3dull, 0x726ull, 0x8c96ull, 0x7f67full, 0x5ac6dbull, 0x34f3ffcull, 0x1a00f6bcull, 0xaf868175ull, 0x409a64fb1ull, 0x14fefa6b32ull, 0x61585a7ca2ull, 0x1959ace5ca3ull, 0x5f8d158983full, 0x1479a92fc0d8ull, 0x4027deeaf5d8ull, 0xb872a0e382cdull, 0x1e83e7cb49679ull, 0x4a97c4d47a8b6ull, 0xa8d0bd7515566ull, 0x162832775e0023ull, 0x2b424a2600aaefull, 0x4ea71273a41f84ull, 0x855e142842d144ull, 0xd32a4a95147601ull, 0x13886456bff8fedull, 0x1b0b9e9f7ff64d2ull, 0x230f0ffe2a9e182ull, 0x2a924a46ea9b667ull, 0x307180a8f94fbb3ull, 0x33ac44f881661d0ull, 0x33ac44f881661d0ull, 0x307180a8f94fbb3ull, 0x2a924a46ea9b667ull, 0x230f0ffe2a9e182ull, 0x1b0b9e9f7ff64d2ull, 0x13886456bff8fedull, 0xd32a4a95147601ull, 0x855e142842d144ull, 0x4ea71273a41f84ull, 0x2b424a2600aaefull, 0x162832775e0023ull, 0xa8d0bd7515566ull, 0x4a97c4d47a8b6ull, 0x1e83e7cb49679ull, 0xb872a0e382cdull, 0x4027deeaf5d8ull, 0x1479a92fc0d8ull, 0x5f8d158983full, 0x1959ace5ca3ull, 0x61585a7ca2ull, 0x14fefa6b32ull, 0x409a64fb1ull, 0xaf868175ull, 0x1a00f6bcull, 0x34f3ffcull, 0x5ac6dbull, 0x7f67full, 0x8c96ull, 0x726ull, 0x3dull, 0x1ull},
                (uint64_t[]){0x1ull, 0x3eull, 0x763ull, 0x93bcull, 0x88315ull, 0x62bd5aull, 0x3aa06d7ull, 0x1d5036b8ull, 0xc9877831ull, 0x4b92cd126ull, 0x1908a0bae3ull, 0x765754e7d4ull, 0x1f6f328d945ull, 0x78e6c26f4e2ull, 0x1a727a885917ull, 0x54a1881ab6b0ull, 0xf89a7fce78a5ull, 0x2a0b11d981946ull, 0x691bac9fc3f2full, 0xf36882498fe1cull, 0x20b53e4eaf5589ull, 0x416a7c9d5eab12ull, 0x79e95c99a4ca73ull, 0xd405269be6f0c8ull, 0x158885ebd574745ull, 0x20bb090011405eeull, 0x2e9402f63fef4bfull, 0x3e1aae9daa94654ull, 0x4da15a4515397e9ull, 0x5b03caefe3eb21aull, 0x641dc5a17ab5d83ull, 0x675889f102cc3a0ull, 0x641dc5a17ab5d83ull, 0x5b03caefe3eb21aull, 0x4da15a4515397e9ull, 0x3e1aae9daa94654ull, 0x2e9402f63fef4bfull, 0x20bb090011405eeull, 0x158885ebd574745ull, 0xd405269be6f0c8ull, 0x79e95c99a4ca73ull, 0x416a7c9d5eab12ull, 0x20b53e4eaf5589ull, 0xf36882498fe1cull, 0x691bac9fc3f2full, 0x2a0b11d981946ull, 0xf89a7fce78a5ull, 0x54a1881ab6b0ull, 0x1a727a885917ull, 0x78e6c26f4e2ull, 0x1f6f328d945ull, 0x765754e7d4ull, 0x1908a0bae3ull, 0x4b92cd126ull, 0xc9877831ull, 0x1d5036b8ull, 0x3aa06d7ull, 0x62bd5aull, 0x88315ull, 0x93bcull, 0x763ull, 0x3eull, 0x1ull},
                (uint64_t[]){0x1ull, 0x3full, 0x7a1ull, 0x9b1full, 0x916d1ull, 0x6b406full, 0x40cc431ull, 0x20fa3d8full, 0xe6d7aee9ull, 0x582b44957ull, 0x1dc1cd8c09ull, 0x8f5ff5a2b7ull, 0x26d4a7dc119ull, 0x9855f4fce27ull, 0x2200e6af4df9ull, 0x6f1402a30fc7ull, 0x14d3c07e92f55ull, 0x3994b9d6691ebull, 0x9326be7945875ull, 0x15c842ee953d4bull, 0x2febc6734853a5ull, 0x621fbaec0e009bull, 0xbb53d937037585ull, 0x14dee83358bbb3bull, 0x22c8d85593e380dull, 0x36438eebe6b4d33ull, 0x4f4f0bf6512faadull, 0x6caeb193ea83b13ull, 0x8bbc08e2bfcde3dull, 0xa8a52534f924a03ull, 0xbf2190915ea0f9dull, 0xcb764f927d82123ull, 0xcb764f927d82123ull, 0xbf2190915ea0f9dull, 0xa8a52534f924a03ull, 0x8bbc08e2bfcde3dull, 0x6caeb193ea83b13ull, 0x4f4f0bf6512faadull, 0x36438eebe6b4d33ull, 0x22c8d85593e380dull, 0x14dee83358bbb3bull, 0xbb53d937037585ull, 0x621fbaec0e009bull, 0x2febc6734853a5ull, 0x15c842ee953d4bull, 0x9326be7945875ull, 0x3994b9d6691ebull, 0x14d3c07e92f55ull, 0x6f1402a30fc7ull, 0x2200e6af4df9ull, 0x9855f4fce27ull, 0x26d4a7dc119ull, 0x8f5ff5a2b7ull, 0x1dc1cd8c09ull, 0x582b44957ull, 0xe6d7aee9ull, 0x20fa3d8full, 0x40cc431ull, 0x6b406full, 0x916d1ull, 0x9b1full, 0x7a1ull, 0x3full, 0x1ull}
            };

        template <size_t B>
        struct block_ordinal
        {
            const size_t N;
            const uint64_t last;

            block_ordinal(const size_t p_N)
                : N(p_N), last(((1ULL << N) - 1) << (B - N))
            {
            }

            uint64_t size() const
            {
                return (1ULL << B);
            }

            uint64_t count() const
            {
                return detail::binom[B][N];
            }

            size_t rank(uint64_t p_x) const
            {
                if (p_x > last)
                {
                    return count();
                }
                size_t b = B;
                size_t n = N;
                uint64_t w = 0;
                uint64_t x = p_x;
                size_t r = 0;
                do
                {
                    // NB once <bit> is readily available,
                    // we can improve the loop by clearing
                    // bits in x, and using
                    //      b = std::bit_width(x)
                    // to drive the loop.

                    uint64_t l = 1ULL << b;
                    uint64_t u = ((1ULL << n) - 1) << (b - n);
                    if (x >= (w|l))
                    {
                        r += detail::binom[b][n];
                        n -= 1;
                        w = (w|l);
                    }
                    else if (x > (w|u))
                    {
                        r += detail::binom[b][n];
                        break;
                    }
                    else if (x == (w|u))
                    {
                        r += detail::binom[b][n] - 1;
                        break;
                    }
                    b -= 1;
                }
                while (b > 0 && b >= n);
                return r;
            }

            uint64_t select(size_t p_r) const
            {
                if (p_r >= count())
                {
                    throw std::runtime_error("select: out of range");
                }
                size_t b = B;
                size_t n = N;
                size_t r = p_r;
                uint64_t x = 0;
                while (r > 0)
                {
                    size_t r0 = detail::binom[b][n];
                    if (r >= r0)
                    {
                        x += 1ULL << b;
                        n -= 1;
                        r -= r0;
                    }
                    b -= 1;
                }
                x |= (1ULL << n) - 1;
                return x;
            }
        };
    }
    // namespace detail
}
// namespace notima

#endif // NOTIMA_RRR_ARRAY_HPP
