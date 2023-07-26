# evaluation-architecture-computers

Repositório dedicado ao desenvolvimento de testes vinculados a arquiteturas de computadores simuladas utilizando Gem5 utilizando como carga algoritmos específicos


# Informações Úteis

## Configuração e Geração de Imagem Linux

- Packer
- QEMU

### Geração de Imagem
    1. Entrar no diretório disk-image/
    2. Executar o ./build.sh
    
    Apõs isso, se a configuração do packer for feita, será gerada uma pasta dentro do diretório disk-image/x86-ubuntu. Dentro dessa pasta será possível encontrar a imagem gerada.

    É recomendável acompanahar a geração da imagem através de um VNC. O console em que foi executado o comando exibirá um IP e uma porta para se conectar.

    Recomenda-se não interagir com a área remota visto que o Packer somado ao QEMU executaram tudo de maneira automatizada.

### Configuração da imagem

A geração das imagens utiliza o [Packer](https://developer.hashicorp.com/packer/tutorials?product_intent=packer). Ele facilita o provisionamento de imagems através do QEMU. O Packer possibilita adicionar os arquivos dentro da imagem, permitindo que eles sejam compilados e executados dentro da emulação Full System.

O template e toda a configuração inicial se baseia no [Recurso](https://resources.gem5.org/resources/x86-ubuntu-18.04-img?version=1.0.0) exposto no Gem5Resources ou diretamente no [repositório](https://github.com/gem5/gem5-resources/tree/develop/src/x86-ubuntu).

Especificamente nessa imagem, ela possui uma configuração em que ela executa os commandos contidos no m5 readfile, viabilizando executar qualquer tipo de comando dentro da emulação. Por conta disso, utilizando o packer, enviamos os arquivos .cpp para dentro da imagem como pode ser observado no arquivo `disk-image/x86-ubuntu/x86-ubuntu.json` na parte de *provisioners*.

Para essa imagem, ela executa diretamente o arquivo `disk-image/x86-ubuntu/post-installation.sh` durante o build da imagem e executa o `disk-image/x86-ubuntu/gem5_init.sh` assim que a imagem sobe na emulação, engatilhando os comandos que do m5 readfile serão executados.

Tendo essas informações em mãos, é possível viablizar um fluxo em que o arquivo que desejamos executar é passado para a imagem através do *provisioner*, colocamos os comandos dentro do m5 readfile que irão compilar e executar o binário gerados.

**É possível passar o binário já compilado e executá-lo, entrando foi encontrado problemas com a versão de compilação que utilizava uma versão diferente de GLIBC, impedindo a execução do binário já compilado.**

## Criação de uma simualação através de componentes Python

O Gem5 viabiliza através de suas abstrações criar qualquer componente desejado, dentre eles:
    - Boards
    - Caches
    - Processors
    - Memory

Existem diversos exemplos de scripts já criados, eles podem ser encontrados ao clonar o projeto do Gem5 e seguir o caminho `gem5/configs/example/gem5_library/

Nesse repositõrio, existe uma pasta chamada `configs`. Ela possui todas as configurações criadas, uma delas é a `x86-ubuntu-run-with-kvm-edited.py`. Essa versão é uma adaptação de uma configuração existente. As alterações feitas foram para utilizar a imagem personalizada, um kernel presente no Gem5Resources e o command que será executado. A variável **command** mantém o conteúdo que será injetado no m5 readfile que foi citado anteriormente, ou seja, ela mantém o comando que será executado apõs o gem5_init foi engatilhado.

Os componentes podem ser criados com base nas abstrações existentes na biblioteca do gem5, elas estã contidas na pasta `/gem5/src/python`. Novos componentes podem ser criados extendendo essas classes. Para utilizá-los deve-se seguir o [tutorial contido na documentação do Gem5](https://www.gem5.org/documentation/gem5-stdlib/develop-own-components-tutorial)

**É importante lembrar de sempre recompilar o binário completo do Gem5 após adicionar um novo componenete a biblioteca existente.**

## Executando o Gem5

    1. É necessário compilar o binário do Gem5 usando o seguinte comando: 
        scons build/X86/gem5.opt -j <number of threads>
    2. Da raiz do Gem5 deve-se executar:
        ./build/X86/gem5.opt path/to/config/file.py
    3. Conectar-se através de telnet ou ssh a simulação FS
    4. Ao final, os resultados da simualação serão gerados no diretório gem5/m5out

