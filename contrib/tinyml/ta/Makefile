CFG_TEE_TA_LOG_LEVEL ?= 4
CFG_TA_OPTEE_CORE_API_COMPAT_1_1=y
TA_DEV_KIT_DIR ?= ~/optee/optee_os/out/arm/export-ta_arm64
TEEC_EXPORT ?= ~/optee/optee_client/out/export

# The UUID for the Trusted Application
BINARY=30d6ec20-bd0b-4461-a8ac-793b6002e1c3

-include $(TA_DEV_KIT_DIR)/mk/ta_dev_kit.mk

ifeq ($(wildcard $(TA_DEV_KIT_DIR)/mk/ta_dev_kit.mk), )
clean:
	@echo 'Note: $$(TA_DEV_KIT_DIR)/mk/ta_dev_kit.mk not found, cannot clean TA'
	@echo 'Note: TA_DEV_KIT_DIR=$(TA_DEV_KIT_DIR)'
endif
