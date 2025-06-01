# Nome do compilador
CC = gcc

# Opções de compilação
CFLAGS = -Wall -g -Iinclude  

# Nome da biblioteca
LIBS = src/libfunc.a

# Arquivos fonte
SRCS = src/func.c src/main.c

# Arquivos objeto
OBJS = $(SRCS:.c=.o)

# Nome do executável
TARGET = eda_fase_2

# Regra padrão
all: $(TARGET)

# Regra para criar o executável
$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LIBS)

# Regra para compilar os arquivos objeto
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Limpar arquivos gerados
clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean