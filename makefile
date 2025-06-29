# Nome do executável
EXEC = teste

# Arquivos fonte
SRC =  VetorFiles.c ListaIndice.c Hash/Hash.c Hash/Contagem.c Hash/RelevanciaHASH.c Patricia/Compara_Patricia.c Patricia/Patricia.c Patricia/Relevancia_Patricia.c main.c

# Compilador
CC = gcc

# Flags de compilação (adicione -g se quiser depuração, ou -Wall para warnings)
CFLAGS = -Wall -I. -I./Hash

# Regra principal
all: $(EXEC)
	@echo Executando o programa:
	./$(EXEC)

# Regra de compilação
$(EXEC): $(SRC)
	$(CC) $(CFLAGS) -o $(EXEC) $(SRC) -lm

# Limpeza dos arquivos compilados
clean:
	rm -f $(EXEC)
