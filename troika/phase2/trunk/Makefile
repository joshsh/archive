################################################################################
## Top-level makefile for the 'Troika' Phase2 interface.

APP         = troika

SRC_DIR     = src/gui
IPK_DIR     = ipk
DOXYGEN_DIR = doxygen


all: x86 ipk doxygen

clean:
	$(MAKE) clean -C $(SRC_DIR)
	$(MAKE) clean -C $(IPK_DIR)

distclean:
	$(MAKE) distclean -C $(SRC_DIR)
	$(MAKE) distclean -C $(IPK_DIR)
	rm -rf $(DOXYGEN_DIR)/html

.PHONY : doxygen
doxygen:
	doxygen $(DOXYGEN_DIR)/phase2.cfg



x86: $(SRC_DIR)/$(APP)

$(SRC_DIR)/$(APP):
	$(MAKE) x86 -C $(SRC_DIR)

x86-clean:
	$(MAKE) x86-clean -C $(SRC_DIR)

x86-distclean:
	$(MAKE) x86-distclean -C $(SRC_DIR)



ipk: $(IPK_DIR)/$(APP).ipk

$(IPK_DIR)/$(APP).ipk: $(SRC_DIR)/$(APP)_arm
	$(MAKE) -C $(IPK_DIR) EXEC=../$(SRC_DIR)/$(APP)_arm

ipk-clean:
	$(MAKE) clean -C $(IPK_DIR)

ipk-distclean:
	$(MAKE) distclean -C $(IPK_DIR)



arm: $(SRC_DIR)/$(APP)_arm

$(SRC_DIR)/$(APP)_arm:
	$(MAKE) arm -C $(SRC_DIR)

arm-clean:
	$(MAKE) arm-clean -C $(SRC_DIR)

arm-distclean:
	$(MAKE) arm-distclean -C $(SRC_DIR)
