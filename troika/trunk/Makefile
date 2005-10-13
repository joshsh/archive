APP	=	troika


all: x86 ipk


x86: ./cpp/$(APP)

./cpp/$(APP):
	cd cpp && make x86


ipk: ./ipk/$(APP).ipk

./ipk/$(APP).ipk: ./cpp/$(APP)_arm
	cd ipk && make EXEC=../cpp/$(APP)_arm


arm: ./cpp/$(APP)_arm

./cpp/$(APP)_arm:
	cd cpp && make arm
