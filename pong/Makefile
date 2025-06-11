# Compiler und Flags
CC         := gcc
CFLAGS     := -Wall -Wextra -Wpedantic -std=c99 -D_POSIX_C_SOURCE=200809L -Isrc
LDFLAGS    := -lncurses -lm

# Verzeichnisse
SRCDIR     := src
BUILDDIR   := build
TESTDIR    := tests

# Unity-Framework (liegt in tests/unity/src)
UNITY_SRC_DIR := $(TESTDIR)/unity/src
UNITY_SRC     := $(UNITY_SRC_DIR)/unity.c
UNITY_INC     := -I$(UNITY_SRC_DIR)

# Programmname
TARGET     := pong

# Quell- und Objekt-Dateien
SRC        := $(wildcard $(SRCDIR)/*.c)
OBJ        := $(patsubst $(SRCDIR)/%.c,$(BUILDDIR)/%.o,$(SRC))

# Alle Modul-Objekte ohne main.o (verhindert doppeltes main)
MODULE_OBJ := $(filter-out $(BUILDDIR)/main.o,$(OBJ))

# Unity-Testquellen und -Binaries (Dateien, die auf _unity.c enden)
UT_SRC     := $(wildcard $(TESTDIR)/*_unity.c)
UT_BIN     := $(patsubst $(TESTDIR)/%.c,$(BUILDDIR)/%,$(UT_SRC))


# Standardziel
all: $(TARGET)

# Linker-Regel für das Hauptprogramm
$(TARGET): $(OBJ)
	$(CC) $^ -o $@ $(LDFLAGS)

# Kompilieren der Objekt-Dateien
$(BUILDDIR)/%.o: $(SRCDIR)/%.c | $(BUILDDIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Build-Verzeichnis anlegen
$(BUILDDIR):
	mkdir -p $(BUILDDIR)

# ------------------------
# Normale (assert-)Tests
# ------------------------
.PHONY: tests
tests: unity-tests

$(TESTDIR)/%: $(TESTDIR)/%.c $(MODULE_OBJ) | $(BUILDDIR)
	$(CC) $(CFLAGS) -Isrc -I$(TESTDIR) $< $(MODULE_OBJ) -o $@

# -----------------------
# Unity-Tests kompilieren
# -----------------------
.PHONY: unity-tests
unity-tests: $(UT_BIN)
	@for t in $(UT_BIN); do \
	  echo "→ $$t"; \
	  ./$$t || exit 1; \
	done
	@echo "✅ Alle Unity-Tests erfolgreich"

# Unity-Test-Binaries bauen (ohne main.o)
$(BUILDDIR)/%: $(TESTDIR)/%.c $(UNITY_SRC) $(MODULE_OBJ) | $(BUILDDIR)
	$(CC) $(CFLAGS) $(UNITY_INC) \
	      $< $(UNITY_SRC) $(MODULE_OBJ) \
	      -o $@ $(LDFLAGS)

# Aufräumen
.PHONY: clean
clean:
	rm -rf $(BUILDDIR) $(TARGET)