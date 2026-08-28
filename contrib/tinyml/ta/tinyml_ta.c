#include <tee_internal_api.h>
#include <tee_internal_api_extensions.h>

#include "include/tinyml_ta.h"
/*
 * TA entry point
 */
TEE_Result TA_CreateEntryPoint(void)
{
    DMSG("TinyML TA: create");

    return TEE_SUCCESS;
}

/*
 * TA destruction
 */
void TA_DestroyEntryPoint(void)
{
    DMSG("TinyML TA: destroy");
}

/*
 * Open session
 */
TEE_Result TA_OpenSessionEntryPoint(
    uint32_t param_types,
    TEE_Param params[4],
    void **sess_ctx)
{
    (void)param_types;
    (void)params;
    (void)sess_ctx;

    DMSG("TinyML TA: session opened");

    return TEE_SUCCESS;
}

/*
 * Close session
 */
void TA_CloseSessionEntryPoint(void *sess_ctx)
{
    (void)sess_ctx;

    DMSG("TinyML TA: session closed");
}

/*
 * Main command handler
 */
TEE_Result TA_InvokeCommandEntryPoint(
    void *sess_ctx,
    uint32_t cmd_id,
    uint32_t param_types,
    TEE_Param params[4])
{
    (void)sess_ctx;

    switch (cmd_id) {

    case TA_TINYML_CMD_INFER:
        /*
         * Expected:
         *
         * params[0] = input
         * params[1] = output
         */

        if (param_types !=
            TEE_PARAM_TYPES(
                TEE_MEM_INPUT,
                TEE_MEM_OUTPUT,
                TEE_PARAM_TYPE_NONE,
                TEE_PARAM_TYPE_NONE)) {

            return TEE_ERROR_BAD_PARAMETERS;
        }

        if (params[0].memref.size != sizeof(struct tinyml_input)) {
            return TEE_ERROR_BAD_PARAMETERS;
        }

        if (params[1].memref.size < sizeof(struct tinyml_output)) {
            return TEE_ERROR_SHORT_BUFFER;
        }

        {
            struct tinyml_input *input =
                (struct tinyml_input *)params[0].memref.buffer;

            struct tinyml_output *output =
                (struct tinyml_output *)params[1].memref.buffer;

            int32_t sum = 0;

            for (uint32_t i = 0;
                 i < input->num_features;
                 i++) {

                sum += input->features[i];
            }

            /*
             * Temporary classifier.
             *
             * This is NOT the neural network yet.
             *
             * We are only testing the secure
             * communication interface.
             */
            if (sum >= 0)
                output->prediction = 1;
            else
                output->prediction = 0;

            output->confidence = sum;

            DMSG(
                "TinyML inference: sum=%d prediction=%d",
                sum,
                output->prediction
            );
        }

        return TEE_SUCCESS;

    case TINYML_CMD_GET_INFO:

        /*
         * Reserved for later.
         */

        return TEE_SUCCESS;

    default:
        return TEE_ERROR_BAD_PARAMETERS;
    }
}
