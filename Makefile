# Имя исполняемого файла

TARGET = replace_v1

# Компилятор

CC = gcc
CFLAGS = -Wall -g -DDEBUG
LDFLAGS = -lsqlite3
# Список всех исходников

SRCS = src/main.c \
		src/rep.c \
		src/sqlite/create_table.c \
		src/sqlite/insert.c \
		src/sqlite/select.c \
		src/sqlite/update.c
	
OBJS = $(SRCS:.c=.o)

#Правило
all: $(TARGET)

# Сборка программы
$(TARGET):$(OBJS)
	@$(CC) $(OBJS) -o $(TARGET) $(LDFLAGS)

# Компиляция каждого файла

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Очистка мусора

clean:
#$(TARGET)
	@rm -f $(OBJS)

# Установка
PREFIX = /usr/local/bin
install: $(TARGET)
	sudo cp $(TARGET) $(PREFIX)
	sudo chmod +x $(PREFIX)/$(TARGET)

