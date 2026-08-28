#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include <tee_client_api.h>

#include "include/tinyml_ta.h"

/*
 * Replace with UUID= 30d6ec20-bd0b-4461-a8ac-793b6002e1c3.
 */
static const TEEC_UUID tinyml_uuid = {
    0x30d6ec20,
    0xbd0b,
    0x4461,
    {0xa8, 0xac,0x79,0x3b,0x60,0x02,0xe1,0xc3}
};

int main(void)
{
    TEEC_Context ctx;
    TEEC_Session session;
    TEEC_Operation op;
    TEEC_Result res;
    uint32_t err_origin;

    struct tinyml_input input;
    struct tinyml_output output;

    /*
     * Initialize input.
     */
    memset(&input, 0, sizeof(input));
    memset(&output, 0, sizeof(output));

    input.num_features = 4;

    input.features[0] = 10;
    input.features[1] = 20;
    input.features[2] = -5;
    input.features[3] = 3;

    /*
     * Initialize TEE context.
     */
    res = TEEC_InitializeContext(NULL, &ctx);

    if (res != TEEC_SUCCESS) {
        printf("TEEC_InitializeContext failed: 0x%x\n", res);
        return 1;
    }

    /*
     * Open session with TinyML TA.
     */
    res = TEEC_OpenSession(
        &ctx,
        &session,
        &tinyml_uuid,
        TEEC_LOGIN_PUBLIC,
        NULL,
        NULL,
        &err_origin
    );

    if (res != TEEC_SUCCESS) {
        printf(
            "TEEC_OpenSession failed: 0x%x origin=0x%x\n",
            res,
            err_origin
        );

        TEEC_FinalizeContext(&ctx);
        return 1;
    }

    /*
     * Prepare operation.
     */
    memset(&op, 0, sizeof(op));

    op.paramTypes = TEEC_PARAM_TYPES(
        TEEC_MEMREF_TEMP_INPUT,
        TEEC_MEMREF_TEMP_OUTPUT,
        TEEC_NONE,
        TEEC_NONE
    );

    op.params[0].tmpref.buffer = &input;
    op.params[0].tmpref.size = sizeof(input);

    op.params[1].tmpref.buffer = &output;
    op.params[1].tmpref.size = sizeof(output);

    /*
     * Invoke TinyML TA.
     */
    res = TEEC_InvokeCommand(
        &session,
        TA_TINYML_CMD_INFER,
        &op,
        &err_origin
    );

    if (res != TEEC_SUCCESS) {
        printf(
            "TEEC_InvokeCommand failed: 0x%x origin=0x%x\n",
            res,
            err_origin
        );

        TEEC_CloseSession(&session);
        TEEC_FinalizeContext(&ctx);

        return 1;
    }

    /*
     * Display result.
     */
    printf("\n");
    printf("===== TinyML Secure Inference =====\n");

    printf("Input features: ");

    for (uint32_t i = 0;
         i < input.num_features;
         i++) {

        printf("%d ", input.features[i]);
    }

    printf("\n");

    printf(
        "Prediction : %d\n",
        output.prediction
    );

    printf(
        "Confidence : %d\n",
        output.confidence
    );

    printf("===================================\n");

    /*
     * Close.
     */
    TEEC_CloseSession(&session);
    TEEC_FinalizeContext(&ctx);

    return 0;
}
