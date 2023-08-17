# gem5-evaluation-computer-architectures

Repository dedicated to the development of tests linked to simulated computer architectures using Gem5, using specific algorithms as the workload.


# Informações Úteis

## Configuration and Linux Image Generation

- Packer
- QEMU

### Image Generation

    1. Enter the disk-image/ directory.
    2. Execute ./build.sh

    
    After this, if the Packer configuration is done, a folder will be generated inside the disk-image/x86-ubuntu directory. Inside this folder, you'll find the generated image.

    It's recommended to monitor the image generation through a VNC. The console where the command was executed will display an IP and port to connect to.

    It's advisable not to interact with the remote area, as Packer along with QEMU will automate everything.

### Image Configuration

The image generation uses [Packer](https://developer.hashicorp.com/packer/tutorials?product_intent=packer). It simplifies image provisioning through QEMU. Packer allows adding files inside the image, enabling them to be compiled and executed within Full System emulation.

The template and all initial configuration are based on the [Resource](https://resources.gem5.org/resources/x86-ubuntu-18.04-img?version=1.0.0) published in Gem5Resources or directly in the[repository](https://github.com/gem5/gem5-resources/tree/develop/src/x86-ubuntu).

Specifically in this image, it has a configuration where it executes the commands contained in m5 readfile, enabling the execution of any type of command within the emulation. Because of this, using Packer, we send the .cpp files into the image as can be seen in the file `disk-image/x86-ubuntu/x86-ubuntu.json` in the *provisioners* section.

For this image, it directly runs the file `disk-image/x86-ubuntu/post-installation.sh` during the image build and executes `disk-image/x86-ubuntu/gem5_init.sh` as soon as the image boots in the emulation, triggering the m5 readfile commands to be executed.

With this information in hand, it's possible to facilitate a workflow where the desired file to execute is passed to the image through the *provisioner*, we put the commands inside m5 readfile that will compile and execute the generated binary.

**It's possible to pass the pre-compiled binary and execute it, although problems were encountered with the compilation version that used a different GLIBC version, preventing the execution of the pre-compiled binary.**

## Creating a Simulation through Python Components

Gem5 makes it possible through its abstractions to create any desired component, including:
- Boards
- Caches
- Processors
- Memory

There are several examples of scripts already created; they can be found by cloning the Gem5 project and following the path `gem5/configs/example/gem5_library/

In this repository, there's a folder named `configs`. Ela possui todas as configurações criadas, uma delas é a `x86-ubuntu-run-with-kvm-edited.py`. This version is an adaptation of an existing configuration. The changes made were to use the custom image, a kernel present in Gem5Resources, and the command that will be executed. The **command** variable maintains the content that will be injected into m5 readfile as mentioned before; in other words, it holds the command that will be executed after gem5_init is triggered.

Components can be created based on the existing abstractions in the gem5 library, which are contained in the `/gem5/src/python` folder. New components can be created by extending these classes. To use them, follow the [tutorial in the Gem5 documentation](https://www.gem5.org/documentation/gem5-stdlib/develop-own-components-tutorial)

**It's important to remember to always recompile the complete Gem5 binary after adding a new component to the existing library.**

## Running Gem5

    1. It's necessary to compile the Gem5 binary using the following command:
        scons build/X86/gem5.opt -j <number of threads>
    2. From the Gem5 root directory, execute:
        ./build/X86/gem5.opt path/to/config/file.py
    3. Connect via telnet or ssh to the FS simulation.
    4. In the end, simulation results will be generated in the gem5/m5out directory.




