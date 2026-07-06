# ⚙️ GRBL H-BOT — Firmware H-BOT (Arduino Mega 2560)

Firmware **GRBL 0.9i** modifié pour piloter le traceur cartésien (CNC pen plotter) du projet H-BOT : cinématique **H-BOT** pour les axes X/Y, axe **Z à crémaillère (rack & pinion)**, drivers **TB6560**, sur carte **Arduino Mega 2560**.

---

## 📋 Sommaire

- [Aperçu](#-aperçu)
- [Particularités de ce firmware](#-particularités-de-ce-firmware)
- [Cycle de homing](#-cycle-de-homing)
- [Architecture des fichiers](#-architecture-des-fichiers)
- [Cartographie des broches (Mega 2560)](#-cartographie-des-broches-mega-2560)
- [Paramètres GRBL ($$)](#-paramètres-grbl-)
- [Installation / Flash](#-installation--flash)
- [Mise en service](#-mise-en-service)
- [Documentation incluse](#-documentation-incluse)
- [Crédits & licence](#-crédits--licence)

---

## 🎯 Aperçu

Ce dépôt contient le code source complet de **Grbl v0.9i** (build `20150620`), adapté pour :

- une cinématique **H-BOT** (`#define H-BOT` dans `config.h`) plutôt qu'une cinématique cartésienne classique ;
- un **Arduino Mega 2560** comme cible (`CPU_MAP_ATMEGA2560`), au lieu de l'Uno/328p officiellement supporté ;
- un axe **Z entraîné par crémaillère** (rack & pinion) pour le levage/pose du stylo, piloté par déplacements G-code (`G1 Z...`) plutôt que par une broche (pas de M3/M5) ;
- des drivers de moteurs pas-à-pas **TB6560**.

Le firmware garde l'intégralité du moteur GRBL d'origine (planificateur trapézoïdal avec look-ahead, interpolation d'arcs, gestion des limites/homing, protocole série temps réel…) ; seules les sections de configuration (`config.h`, `cpu_map.h`) et les valeurs par défaut (`defaults/`, `settings.txt`) sont adaptées à la machine.

---

## 🧩 Particularités de ce firmware

| Aspect | Configuration |
|---|---|
| Cinématique | **H-BOT** activée (`#define H-BOT`, `config.h`) |
| Carte cible | Arduino **Mega 2560** (`CPU_MAP_ATMEGA2560`) |
| Axe Z | Crémaillère / pignon, contrôlé par G-code (Z up/down), pas de broche |
| Verrouillage au démarrage | `HOMING_INIT_LOCK` actif : la machine part en `ALARM` tant qu'un homing (`$H`) n'a pas été effectué |
| Origine machine | `HOMING_FORCE_SET_ORIGIN` : l'origine (0,0,0) est fixée à la position de homing, quelle que soit l'orientation des switches |
| Cycles de localisation | `N_HOMING_LOCATE_CYCLE = 2` (2 passes de recherche pour plus de répétabilité) |

---

## 🏠 Cycle de homing

Contrairement au firmware GRBL standard (qui home généralement X et Y ensemble puis Z), ce build définit un **ordre séquentiel personnalisé**, adapté à la géométrie H-BOT de la machine :

```c
// config.h
#define HOMING_CYCLE_0 (1<<Z_AXIS)  // 1) Home Z en premier, pour dégager le stylo
#define HOMING_CYCLE_1 (1<<X_AXIS)  // 2) Puis home X (pull-off pur en X sur H-BOT)
#define HOMING_CYCLE_2 (1<<Y_AXIS)  // 3) Puis home Y (pull-off pur en Y sur H-BOT)
```

**Pourquoi cet ordre ?**
1. **Z d'abord** : la crémaillère remonte le stylo pour dégager le plan de dessin avant tout déplacement latéral, évitant de rayer le support ou de heurter un obstacle.
2. **X puis Y séparément** (au lieu d'un homing X+Y simultané) : sur une cinématique H-BOT, homer les deux moteurs en même temps produirait un mouvement diagonal des deux courroies, rendant le pull-off ambigu. En les séparant, chaque axe applique un pull-off **pur** sur sa propre direction, ce qui fiabilise la position finale.

Les réglages associés dans `settings.txt` / `$$` :

| Paramètre | Rôle | Valeur par défaut |
|---|---|---|
| `$22` | Cycle de homing activé | `1` |
| `$23` | Masque d'inversion de direction du homing | `0` |
| `$24` | Vitesse de homing (locate), mm/min | `100.0` |
| `$25` | Vitesse de homing (seek), mm/min | `500.0` |
| `$26` | Anti-rebond des switches, ms | `250` |
| `$27` | Distance de pull-off (unique, tous axes), mm | `2.0` |
| `$28` | Mode de homing (`0` = recherche + localisation, `1` = recherche + pull-off simple) | `0` |

> ℹ️ Dans cette version, la distance de pull-off (`$27`) est **globale** (un seul réglage pour les trois axes), contrairement à des variantes plus récentes du projet qui introduisent des pull-off indépendants par axe.

---

## 🗂️ Architecture des fichiers

```
biblio_finale/
├── main.c                    # Point d'entrée, boucle principale Grbl
├── config.h                  # Configuration de compilation (H-BOT, homing, Mega2560…)
├── cpu_map.h                 # Sélecteur de cartographie CPU
├── cpu_map/
│   ├── cpu_map_atmega2560.h  # Brochage Arduino Mega 2560 (utilisé ici)
│   └── cpu_map_atmega328p.h  # Brochage Arduino Uno (référence GRBL standard)
├── defaults.h                # Sélecteur des réglages par défaut
├── defaults/
│   ├── defaults_generic.h    # Profil générique (utilisé par défaut, DEFAULTS_GENERIC)
│   └── defaults_*.h          # Profils de référence pour d'autres machines (Shapeoko, X-Carve…)
├── settings.c / settings.h   # Lecture/écriture des paramètres $ (EEPROM)
├── settings.txt              # Jeu de réglages $ prêt à envoyer, commenté, pour cette machine
├── limits.c / limits.h       # Gestion des fins de course et du cycle de homing
├── stepper.c / stepper.h     # Génération des impulsions de pas (Bresenham + AVR timers)
├── planner.c / planner.h     # Planification des mouvements, look-ahead
├── motion_control.c/.h       # Traduction des mouvements G-code en segments planifiés
├── gcode.c / gcode.h         # Parseur G-code (RS274/NGC)
├── protocol.c / protocol.h   # Boucle de traitement des lignes série / commandes temps réel
├── report.c / report.h       # Messages de statut, réponses `?`, `$$`…
├── serial.c / serial.h       # Communication série (USART)
├── eeprom.c / eeprom.h       # Accès EEPROM (réglages persistants)
├── probe.c / probe.h         # Sonde (non utilisée ici, stylo ≠ broche)
├── spindle_control.c/.h      # Contrôle broche/PWM (hérité, non utilisé pour le stylo)
├── coolant_control.c/.h      # Contrôle arrosage (hérité, non utilisé)
├── system.c / system.h       # État système, pin change interrupts (contrôle, limites)
├── nuts_bolts.h / .c          # Constantes et utilitaires généraux, kinématique H-BOT
├── print.c / print.h         # Formatage des sorties série
├── examples/grblUpload/      # Sketch Arduino pour flasher Grbl depuis l'IDE
├── grbl_plotter_setup.md     # Guide de configuration logicielle (GRBL Plotter)
└── wiring_mega.md            # Plan de câblage de référence (Mega 2560 + TB6560)
```

---

## 🔌 Cartographie des broches (Mega 2560)

D'après `cpu_map/cpu_map_atmega2560.h` (source de vérité utilisée par le firmware compilé) :

| Fonction | Port AVR | Bit | Broche Mega 2560 |
|---|---|---|---|
| Step X | `PORTA` | 2 | D24 |
| Step Y | `PORTA` | 3 | D25 |
| Step Z | `PORTA` | 4 | D26 |
| Direction X | `PORTC` | 7 | D30 |
| Direction Y | `PORTC` | 6 | D31 |
| Direction Z | `PORTC` | 5 | D32 |
| Stepper enable (commun) | `PORTB` | 7 | D13 |
| Limite X | `PORTB` | 4 | D10 |
| Limite Y | `PORTB` | 5 | D11 |
| Limite Z | `PORTB` | 6 | D12 |
| Reset / Feed hold / Cycle start / Safety door | `PORTK` | 0–3 | A8–A11 |
| Probe | `PORTK` | 7 | A15 |

> 📄 Un plan de câblage détaillé, orienté drivers TB6560, est fourni dans [`wiring_mega.md`](./wiring_mega.md). Si ta plaque utilise un brochage différent (ex. STEP sur A0/A1/A2), vérifie qu'il correspond bien à `cpu_map_atmega2560.h` avant de flasher — sinon adapte ce fichier à ton câblage réel.

---

## 🔧 Paramètres GRBL ($$)

Le fichier [`settings.txt`](./settings.txt) contient le jeu de réglages `$` commenté, prêt à être envoyé ligne par ligne (uniquement les lignes commençant par `$`) via un moniteur série ou GRBL Plotter :

| $ | Description | Valeur |
|---|---|---|
| `$0` | Longueur d'impulsion de pas (µs) | `10` |
| `$1` | Délai de mise au repos des moteurs (ms) | `255` (maintien permanent) |
| `$2` / `$3` | Masques d'inversion pas / direction | `0` |
| `$4` | Inversion enable moteur (TB6560 = actif bas) | `0` |
| `$5` | Inversion des fins de course | `0` |
| `$10` | Masque de rapport de statut (position machine + travail) | `3` |
| `$11` | Déviation de jonction (mm) | `0.010` |
| `$12` | Tolérance d'arc (mm) | `0.002` |
| `$20` / `$21` | Limites logicielles / matérielles | `0` / `1` |
| `$22`–`$28` | Cycle de homing *(voir section dédiée ci-dessus)* | — |
| `$100` / `$101` | Pas/mm courroies H-BOT (X, Y) | `80.000` |
| `$102` | Pas/mm crémaillère Z | `50.930` |
| `$110`–`$112` | Vitesses max (mm/min) X, Y, Z | `3000 / 3000 / 300` |
| `$120`–`$122` | Accélérations (mm/s²) X, Y, Z | `100 / 100 / 50` |
| `$130`–`$132` | Course max (mm) X, Y, Z | `300 / 300 / 50` |

> Les formules de calcul des pas/mm (moteur × micro-pas ÷ poulie/pignon) sont documentées directement en commentaire dans `settings.txt`.

---

## 🚀 Installation / Flash

1. **Importer Grbl dans l'IDE Arduino** : suivre la procédure standard GRBL (bibliothèque `Grbl` installée depuis ce dossier source).
2. Ouvrir `examples/grblUpload/grblUpload.ino` dans l'IDE Arduino.
3. Sélectionner la carte **Arduino Mega 2560** et le bon port série.
4. Cliquer sur **Upload** — le sketch compile et flashe l'intégralité du firmware (`config.h` inclus).
5. Envoyer les réglages de [`settings.txt`](./settings.txt) (lignes `$` uniquement), puis vérifier avec `$$`.

## ▶️ Mise en service

Le guide complet est dans [`grbl_plotter_setup.md`](./grbl_plotter_setup.md) ; résumé rapide :

1. Connexion série à **115200 bauds**, vérifier le message d'accueil `Grbl 0.9i`.
2. Envoyer les réglages `$` puis `$$` pour vérifier.
3. Au démarrage, la machine est en `ALARM` (verrouillage de sécurité) → lancer le homing (`$H`).
4. Une fois homée, l'origine machine est fixée à (0, 0, 0).
5. Piloter le stylo par déplacements Z (`G1 Z5 F300` levé, `G1 Z0 F300` posé) — pas de commande broche.
6. Tester quelques jogs X/Y avant un tracé complet pour confirmer le sens des axes H-BOT (inverser `$3` si besoin).

---

## 📚 Documentation incluse

| Fichier | Contenu |
|---|---|
| [`grbl_plotter_setup.md`](./grbl_plotter_setup.md) | Configuration du logiciel de pilotage (GRBL Plotter) : connexion, envoi des réglages, homing, contrôle du stylo |
| [`wiring_mega.md`](./wiring_mega.md) | Plan de câblage Mega 2560 ↔ drivers TB6560 ↔ fins de course |
| [`settings.txt`](./settings.txt) | Jeu de réglages `$` commenté, prêt à l'emploi |

---

## 🙏 Crédits & licence

Basé sur :
- [grbl/grbl](https://github.com/grbl/grbl) — Grbl officiel, © Sungeun (Sonny) K. Jeon & Simen Svale Skogsrud
- [robottini/grbl-servo](https://github.com/robottini/grbl-servo)
- Guide DIY d'Arnab Kumar Das ([arnabkumardas.com](http://www.arnabkumardas.com)) pour la variante H-BOT + servo

Grbl est un logiciel libre distribué sous licence **GPLv3**.

---

<p align="center"><sub>Projet d'ingénierie — Firmware H-BOT pour traceur cartésien DRAWBOT</sub></p>
