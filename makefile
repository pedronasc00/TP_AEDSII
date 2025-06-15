# Nome do executável
EXEC = teste

# Arquivos fonte
SRC = Tabela_Hash/Hash.c ListaIndice.c Tabela_Hash/Contagem.c main.c

# Compilador
CC = gcc

# Flags de compilação (adicione -g se quiser depuração, ou -Wall para warnings)
CFLAGS = -Wall -I. -I./Tabela_Hash

# Regra principal
all: $(EXEC)
	@echo Executando o programa:
	./$(EXEC)

# Regra de compilação
$(EXEC): $(SRC)
	$(CC) $(CFLAGS) -o $(EXEC) $(SRC)

# Limpeza dos arquivos compilados
clean:
	rm -f $(EXEC)
