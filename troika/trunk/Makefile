################################################################################
## Top-level makefile for the 'Troika' Phase2 interface.

APP	=	troika



all: x86 ipk

clean:
	$(MAKE) clean -C cpp
	$(MAKE) clean -C ipk

distclean:
	$(MAKE) distclean -C cpp
	$(MAKE) distclean -C ipk



x86: ./cpp/$(APP)

./cpp/$(APP):
	$(MAKE) x86 -C cpp

x86-clean:
	$(MAKE) x86-clean -C cpp

x86-distclean:
	$(MAKE) x86-distclean -C cpp



ipk: ./ipk/$(APP).ipk

./ipk/$(APP).ipk: ./cpp/$(APP)_arm
	$(MAKE) -C ipk EXEC=../cpp/$(APP)_arm

ipk-clean:
	$(MAKE) clean -C ipk

ipk-distclean:
	$(MAKE) distclean -C ipk



arm: ./cpp/$(APP)_arm

./cpp/$(APP)_arm:
	$(MAKE) arm -C cpp

arm-clean:
	$(MAKE) arm-clean -C cpp

arm-distclean:
	$(MAKE) arm-distclean -C cpp
