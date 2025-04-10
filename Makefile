# Flag de compilation avec le debug possible
CFLAGS= -c -g  -Iinclude -Wall
# Flag pour l'édition des liens, juste la bibliothèque math a ajouter
LDFLAGS= -lm

# Le repertoire des modules sources (pas de main)
SRC_DIR=src
# Les programmes de test et finaux (avec un main pour chaque fichier)
EXE_DIR=tests
# Le repertoire contenant les fichiers objets
OBJ_DIR=obj
# Le repertoire contenant les fichiers exécutables
BIN_DIR=bin
# Le repertoire contenant les fichiers d'en-tête
INCLUDE_DIR=include

# SRC_FILES : Fichiers contenant tous les modules sources .c, ne contenant aucun main
SRC_FILES=$(wildcard $(SRC_DIR)/*.c)
#SRC_FILES := $(filter-out $(EXE_FILES), $(SRC_FILES))

# OBJ_FILES : Fichiers contenant tous les fichiers objets .o, mais uniquement des modules ne contenant aucun main
O1=$(patsubst %.c, %.o, $(SRC_FILES))
OBJ_FILES=$(patsubst $(SRC_DIR)/%, $(OBJ_DIR)/%, $(O1))

# B1 : Fichiers contenant des main qui sont dans le répertoire tests
B1= $(wildcard $(EXE_DIR)/*.c)

# EXE_FILES : Les fichiers programmes exécutables
EXE_FILES=$(patsubst $(EXE_DIR)/%.c, $(BIN_DIR)/%, $(B1))

# Règle par défaut : on crée tous les exécutables du répertoire tests
all: $(EXE_FILES)

# Exemple d'une règle pour construire un exécutable nommé exemple1. exemple1.c doit se trouver dans le répertoire tests. Cette reèle construit bin/exemple1 à partir de obj/exemple1.o et des autres fichiers objets .o issus des fichiers contenant les fonctions dans le répertoire src
$(BIN_DIR)/exemple1 : $(OBJ_DIR)/exemple1.o  $(OBJ_FILES)
	gcc  $^ -o $@ $(LDFLAGS)

# Règle pour les autres programmes
$(BIN_DIR)/% :  $(OBJ_FILES) $(OBJ_DIR)/%.o
	gcc  $^ -o $@ $(LDFLAGS)

# Règle pour construire les fichiers objets .o à partir des .c pour les modules sans main
$(OBJ_DIR)/%.o : $(SRC_DIR)/%.c
	gcc $(CFLAGS) -I$(INCLUDE_DIR) $^ -o $(OBJ_DIR)/$*.o

# Règle pour construire les fichiers objets .o à partir des .c pour les programmes avec main
$(OBJ_DIR)/%.o : $(EXE_DIR)/%.c
	gcc $(CFLAGS) -I$(INCLUDE_DIR) $^ -o $@

clean :
	rm -rf $(OBJ_DIR)/*.o $(OBJ_FILES) $(EXE_FILES) $(INCLUDE_DIR)/*.gch $(BIN_DIR)/*.dSYM
