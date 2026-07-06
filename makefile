
SRCS := \
	src/Domain/Admin.cpp \
	src/Domain/Customer.cpp \
	src/Domain/Drink.cpp \
	src/Domain/Food.cpp \
	src/Domain/Gold.cpp \
	src/Domain/Menu.cpp \
	src/Domain/MenuItem.cpp \
	src/Domain/MembershipLevel.cpp \
	src/Domain/Normal.cpp \
	src/Domain/Silver.cpp \
	src/Domain/VIP.cpp \
	src/Domain/Order.cpp \
	src/Domain/Restaurant.cpp \
	src/Domain/Restaurateur.cpp \
	src/Repository/AdminStorage.cpp \
	src/Repository/CustomerStorage.cpp \
	src/Repository/MenuStorage.cpp \
	src/Repository/OrderStorage.cpp \
	src/Repository/RestaurantStorage.cpp \
	src/Repository/RestaurateurStorage.cpp \
	src/Database/DatabaseManager.cpp \
	src/UI/GetInformation.cpp \
	src/UI/Interface.cpp \
	src/UI/Printer.cpp \
	src/Utility/IDGenerator.cpp \
	src/main.cpp

TARGET := bin/krusty_krab
CXXFLAGS := -std=c++14 -Wall -Wextra -Iinclude -g -O0
LDLIBS := -lsqlite3

.PHONY: all clean test

all: $(TARGET)

$(TARGET): $(SRCS)
	mkdir bin/ -p
	g++ $(CXXFLAGS) $(SRCS) -o $(TARGET) $(LDLIBS)

clean:
	rm -f $(TARGET) bin/customer_order_status_test
