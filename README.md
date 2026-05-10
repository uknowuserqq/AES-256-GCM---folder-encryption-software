# 🛡️ C++ AES-256 - Folder Encrypter

> [!CAUTION]
> ### ⚠️ ALERTA:
> Al ejecutar este programa, **todo el contenido** dentro de la carpeta objetivo será encriptado mediante AES-256-GCM y **Despues borrado**. 
>
> **Expansión de carpetas :**
> Sobre las carpetas dentro de otras: El programa es "Expansivo". Si adentro de tu carpeta hay más carpetas, el código se va a meter en cada una y va a encriptar todos los archivos que encuentre.
---

### 🚀 Pasos 

1. **Descarga el archivo**
   Descarga el archivo `Encrypt.cpp` 

2. **Configuración**
   Abre el archivo y dirígete a la **línea 53**. Modifica la ruta de la siguiente manera:
   ```cpp
   std::string carpeta_objetivo = "Aca pone el nombre de tu carpeta a encriptar";
Guarda los cambios (Ctrl+S)

Abri una terminal
   
Entrar a la carpeta donde este el archivo "Ransomware.cpp": cd [Ruta de la carpeta]

Compilar: g++ -std=c++17 Encrypt.cpp -o Encrypt -lssl -lcrypto

Ejecutar: ./Encrypt
