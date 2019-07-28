#
#  Copyright (C) 2019 Androsov Alexander <eelleekk@gmail.com>
#
#  This program is free software; you can redistribute it and/or modify
#  it under the terms of the GNU General Public License version 2 as
#  published by the Free Software Foundation.
#

PRODUCT_PACKAGES += \
	vendor.globallogic.ledcontrol@1.0-service.hikey960 \

TARGET_FS_CONFIG_GEN += \
	vendor/globallogic/config/config.fs

DEVICE_MANIFEST_FILE += \
	vendor/globallogic/interfaces/manifest.xml

BOARD_SEPOLICY_DIRS += \
	vendor/globallogic/sepolicy

DEVICE_MATRIX_FILE += \
	vendor/globallogic/interfaces/compatibility_matrix.xml