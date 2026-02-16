# Définition du compilateur et des flags
CC = gcc
CFLAGS = -std=c11 -O2

# Liste des jours
DAYS = 1 2 3 4 5 6 7 8 9 10 11 12 

# Cible par défaut : compile et exécute tous les jours
all: $(DAYS)

# Règle pour chaque jour
$(DAYS):
	@echo "=== Compilation et exécution du Jour $@ ==="
	@if [ -d "Jour_$@" ]; then \
		cd Jour_$@ && \
		$(CC) $(CFLAGS) day_$@.c -o day_$@ && \
		echo "Exécution de day_$@:" && \
		./day_$@; \
	else \
		echo "Erreur: Le dossier Jour_$@ n'existe pas"; \
	fi
	@echo ""

# Nettoyer les exécutables
clean:
	@echo "Nettoyage des exécutables..."
	@for i in $(DAYS); do \
		if [ -f "Jour_$$i/day_$$i" ]; then \
			rm -f Jour_$$i/day_$$i; \
			echo "Supprimé: Jour_$$i/day_$$i"; \
		fi; \
	done

# Compiler sans exécuter
compile: $(addprefix compile_, $(DAYS))

compile_%:
	@echo "=== Compilation du Jour $* ==="
	@if [ -d "Jour_$*" ]; then \
		cd Jour_$* && $(CC) $(CFLAGS) day_$*.c -o day_$@; \
		echo "Compilé: Jour_$*/day_$*"; \
	else \
		echo "Erreur: Le dossier Jour_$* n'existe pas"; \
	fi

# Exécuter un jour spécifique (usage: make run DAY=3)
run:
	@if [ -z "$(DAY)" ]; then \
		echo "Usage: make run DAY=<numéro>"; \
		exit 1; \
	fi
	@if [ -d "Jour_$(DAY)" ]; then \
		cd Jour_$(DAY) && ./day_$(DAY); \
	else \
		echo "Erreur: Le dossier Jour_$(DAY) n'existe pas"; \
	fi

.PHONY: all clean compile run $(DAYS)
