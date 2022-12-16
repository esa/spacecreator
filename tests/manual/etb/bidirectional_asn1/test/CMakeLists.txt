cmake_minimum_required(VERSION 3.16)
project(bidirectional VERSION 1.0.0 LANGUAGES C)

add_executable(bidirectional)
target_sources(bidirectional
    PRIVATE     main.c
                asn1crt.c
                asn1crt_encoding.c
                asn1crt_encoding_acn.c
                asn1crt_encoding_uper.c
                bidirectional_enc.c
                bidirectional_dec.c
)
