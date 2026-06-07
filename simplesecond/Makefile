# Имя исполняемого файла

TARGET = simplesecond

# Компилятор

CC = gcc
CFLAGS = -Wall -g -DDEBUG
LDFLAGS = -lsqlite3
# Список всех исходников

SRCS = src/main.c \
		src/init_app.c \
		src/aux/auxilary_func.c \
		src/simplesecond.c \
		src/rep_find.c \
		src/rep_link.c \
		src/sqlite/create_table.c \
		src/sqlite/select.c \
		src/sqlite/insert.c \
		src/sqlite/update.c \
		src/ui/ui.c \
		src/handle/handle.c
		
	
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

