#ifndef TINYML_TA_H
#define TINYML_TA_H

#include <stdint.h>

/*
 * UUID:
 * 268dcfe5-557f-4867-a99a-05b6aab2a89b
 */

#define TA_TINYML_UUID \
	 {0x30d6ec20, 0xbd0b, 0x4461, {0xa8, 0xac,0x79,0x3b,0x60,0x02,0xe1,0xc3}}

/* Commands */
#define TA_TINYML_CMD_INFER		0
#define TINYML_CMD_GET_INFO    1

/*
 * Maximum number of input features
 *
 * We are deliberately starting small.
 * Later this can match the actual TinyML model.
 */
#define TINYML_MAX_FEATURES    16

/*
 * Input structure
 */
struct tinyml_input {
    uint32_t num_features;
    int8_t features[TINYML_MAX_FEATURES];
};

/*
 * Output structure
 */
struct tinyml_output {
    int32_t prediction;
    int32_t confidence;
};

#endif
