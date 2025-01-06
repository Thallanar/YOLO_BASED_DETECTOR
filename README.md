# YOLO_BASED_DETECTOR

 Um código simples de detecção de pessoas para anotações.

## COMO USAR

-> Crie os diretórios padrões do código:

    ``bash         mkdir /CAMERA /output /settings     ``

-> Instale o OpenCV e suas depêndencias:

#### Windows:

**Usando Pré-compilados:**

1. Baixe os binários pré-compilados:

- Acesse o site oficial do OpenCV: https://opencv.org/releases/;
- Baixe o arquivo .exe ou .zip correspondente à versão desejada (ex.: `OpenCV 4.x`);

<br>

2. Configure o ambiente:

- Extraia os arquivos em um local, como `C:\opencv`;
- Adicione o caminho `C:\opencv\build\x64\vc15\bin` (ou equivalente) à variável de ambiente `PATH`;

<br>

3. Configure no Visual Studio ou CMake:

- No Visual Studio: Adicione o caminho para os includes (`include`) e bibliotecas (`lib`);
- Compile com as bibliotecas necessárias (ex.: `opencv_world4xx.lib`).

**Usando CMake e Código-Fonte:**

1. Instale o CMake: https://cmake.org/;
   
<br>

2. Baixe o código-fonte do OpenCV:

- Clone o repositório:
  ```bash
      git clone https://github.com/opencv/opencv.git
  ```

<br>

3. Configure com o CMake:

   ```bash
       cmake -S . -B build -G "Visual Studio 16 2019"
   ```

<br>

4. Compile usando o Visual Studio ou o comando:

   ```bash
        cmake --build build --config Release
   ```

<br>

### Ubuntu/Debian (recomendado)

**Usando gerenciador de pacotes `apt` (mais simples e recomendado para yolov4):**

1. Atualize os pacotes (opcional):

   ```bash
       sudo apt update && sudo apt -y upgrade
   ```

<br>

2. Instale o OpenCV com as dependências:

   ```bash
       sudo apt install libopencv-dev \
                        python3-opencv
   ```

<br>

**Compilando coom o Código-Fonte (mais avançado):**

1. Instale as dependências:

   ```bash
       sudo apt install build-essential \ 
                        cmake \
                        git \
                        libgtk-3-dev \
                        libcanberra-gtk3-module \
                        libtbb2 \
                        libtbb-dev \
                        libjpeg-dev \
                        libpng-dev \
                        libtiff-dev \
                        libdc1394-22-dev
   ```

> **obs:** Note que algumas bibliotecas irão variar conforme a versão de seu sistema operacional! Faça alterações no código, caso necessário.

<br>

2. Baixe o código-fonte:

   ```bash
       git clone https://github.com/opencv/opencv.git && \
       git clone https://github.com/opencv/opencv_contrib.git
   ```

<br>

3. Crie o diretório de build

    ```bash
        cd opencv && \
        mkdir build && cd build
    ```

<br>

4. Configure o CMake:

    ```bash
        cmake -D WITH_HIGHUI=ON \
              -D WITH_GSTREAM=ON \
              -D WITH_FFMPEG \
              -D CMAKE_BUILD_TYPE=Release \
              -D CMAKE_INSTALL_PREFIX=/usr/local \
              -D OPENCV_EXTRA_MODULES_PATH=../../opencv_contrib/modules ..
    ```

> **obs:** em alguns casos, `sudo` será necessário.

<br>

5. Compile e instale:
    
    ```bash
        sudo make -j$(nproc)
    ```
    ```bash
        sudo make install
    ```
