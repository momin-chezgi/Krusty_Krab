
SRCS := \
	src/Domain/Admin.cpp \
	src/Domain/Customer.cpp \
	src/Domain/Drink.cpp \
	src/Domain/Food.cpp \
	src/Domain/Menu.cpp \
	src/Domain/MenuItem.cpp \
	src/Domain/Order.cpp \
	src/Domain/Restaurant.cpp \
	src/Domain/Restaurateur.cpp \
	src/Repository/AdminStorage.cpp \
	src/Repository/CustomerStorage.cpp \
	src/Repository/MenuStorage.cpp \
	src/Repository/OrderStorage.cpp \
	src/Repository/RestaurantStorage.cpp \
	src/Repository/RestaurateurStorage.cpp \
	src/Repository/SQL.cpp \
	src/Database/DatabaseManager.cpp \
	src/UI/GetInformation.cpp \
	src/UI/Interface.cpp \
	src/UI/Printer.cpp \
	src/Utility/IDGenerator.cpp \
	src/main.cpp

TARGET := bin/krusty_krab
CXXFLAGS := -std=c++14 -Wall -Wextra -Iinclude -g -O0
LDLIBS := -lsqlite3

HYDRATION_TEST_SRCS := \
	tests/menu_item_hydration_test.cpp \
	src/Domain/Drink.cpp \
	src/Domain/Food.cpp \
	src/Domain/MenuItem.cpp \
	src/Utility/IDGenerator.cpp

ORDER_SNAPSHOT_TEST_SRCS := \
	tests/order_snapshot_test.cpp \
	src/Domain/Drink.cpp \
	src/Domain/Food.cpp \
	src/Domain/MenuItem.cpp \
	src/Domain/Menu.cpp \
	src/Domain/Order.cpp \
	src/Repository/MenuStorage.cpp \
	src/Repository/OrderStorage.cpp \
	src/Database/DatabaseManager.cpp \
	src/Utility/IDGenerator.cpp

.PHONY: all clean test

all: $(TARGET)

$(TARGET): $(SRCS)
	mkdir bin/ -p
	g++ $(CXXFLAGS) $(SRCS) -o $(TARGET) $(LDLIBS)

test: bin/menu_item_hydration_test bin/order_snapshot_test
	./bin/menu_item_hydration_test
	./bin/order_snapshot_test

bin/menu_item_hydration_test: $(HYDRATION_TEST_SRCS)
	mkdir bin/ -p
	g++ $(CXXFLAGS) $(HYDRATION_TEST_SRCS) -o bin/menu_item_hydration_test

bin/order_snapshot_test: $(ORDER_SNAPSHOT_TEST_SRCS)
	mkdir bin/ -p
	g++ $(CXXFLAGS) $(ORDER_SNAPSHOT_TEST_SRCS) -o bin/order_snapshot_test $(LDLIBS)

clean:
	rm -f $(TARGET) bin/menu_item_hydration_test bin/order_snapshot_test
