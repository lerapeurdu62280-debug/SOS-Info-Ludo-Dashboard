# 🛡️ SOS INFO LUDO - SYSTEM MONITORING SUITE

<div align="center">

![Version](https://img.shields.io/badge/version-1.0.0-blue?style=for-the-badge)
![Platform](https://img.shields.io/badge/platform-Windows_10_%2F_11-0078D6?style=for-the-badge&logo=windows&logoColor=white)
![Language](https://img.shields.io/badge/C%2B%2B-Win32_API-00599C?style=for-the-badge&logo=c%2B%2B&logoColor=white)
![License](https://img.shields.io/badge/license-Proprietary-red?style=for-the-badge)

**L'outil ultime de diagnostic matériel et réseau pour Windows.**
*Ultra-léger. Sans installation. Temps réel.*

[⬇️ TÉLÉCHARGER LA DERNIÈRE VERSION](https://github.com/lerapeurdu62280-debug/SOS-Info-Ludo-Dashboard/releases)

</div>

---

## 📸 Aperçu

 ![Aperçu du Logiciel](capture.png)

---

## ⚡ Pourquoi cet outil ?

Développé exclusivement pour **SOS INFO LUDO**, ce logiciel remplace les gestionnaires de tâches classiques. Il est conçu pour offrir une visibilité immédiate sur la santé d'un PC sans alourdir le système.

Contrairement aux solutions commerciales lourdes, **SYS_PRO** utilise directement le noyau Windows (Kernel) et le Registre pour extraire les données, garantissant une empreinte mémoire minime (< 5 Mo).

## 🚀 Fonctionnalités Clés

### 🖥️ Dashboard & Monitoring
- **CPU & RAM en temps réel :** Graphiques vectoriels fluides avec historique de charge.
- **Réseau :** Analyse instantanée de la bande passante (Upload/Download) via `iphlpapi`.
- **Zéro Latence :** Rafraîchissement des données à 20Hz (50ms).

### 🌡️ Sondes & Capteurs (Smart Sensing)
- **Fréquence CPU :** Lecture directe de la sonde `~MHz` via le Registre.
- **Température CPU (Est.) :** Algorithme de simulation thermique intelligent pour les processeurs bridés (AMD/Intel) sans pilote ring-0.
- **Ventilation :** Détection de l'état du refroidissement.

### 🔍 Deep Hardware Scan
Identifie précisément les composants sans ouvrir le PC :
| Composant | Méthode de détection |
| :--- | :--- |
| **Processeur** | Registre Système (Hardware Description) |
| **Carte Graphique** | API Display Windows |
| **Carte Mère** | BaseBoard Product ID |
| **BIOS** | SMBIOS Version |

---

## 🛠️ Stack Technique

Ce projet est une prouesse d'optimisation, codé en **C++ Natif** sans aucun moteur graphique externe (pas de Qt, pas d'Electron).

* **GUI Engine :** Moteur de rendu graphique personnalisé (GDI+) avec support de l'anti-aliasing, des dégradés et de la transparence alpha.
* **Architecture :** Win32 API pure (Performance maximale).
* **Système :** Multi-threading pour le scan matériel (ne bloque jamais l'interface).

## 📦 Installation & Utilisation

Ce logiciel est **Portable**. Il ne nécessite aucune installation.

1.  Allez dans la section **[Releases]** (à droite de la page GitHub).
2.  Téléchargez le fichier `SOS_System_Check.exe`.
3.  **IMPORTANT :** Faites `Clic-droit` > `Exécuter en tant qu'administrateur` pour permettre l'accès aux sondes thermiques.

## 🔒 Sécurité

Le code source est entièrement ouvert et auditable ici même.
- Aucune collecte de données vers l'extérieur.
- Aucun processus caché.
- Nettoyage automatique de la mémoire à la fermeture.

---

<div align="center">

**Développé avec passion par SOS INFO LUDO**
*Expertise & Diagnostic Informatique*

© 2026 Tous droits réservés.

</div>
