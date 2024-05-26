import gem5
from gem5 import *
from m5.objects import *

# Função para criar uma cache com tamanho e associatividade configuráveis
def create_cache(size, assoc):
    return L1Cache(size=size, assoc=assoc)



DISK_PATH = "/home/guilherme.dantas/disk-images/x86-ubuntu"


# Configuração da plataforma (arquitetura e sistema operacional)
system = System()
system.clk_domain = SrcClockDomain()
system.clk_domain.clock = '3GHz'
system.clk_domain.voltage_domain = VoltageDomain()

system.mem_mode = 'timing'  # Modo de memória: timing
system.mem_ranges = [AddrRange('3GB')]  # Faixa de endereços de memória

# Configuração do processador
system.cpu = TimingSimpleCPU()  # CPU simples com execução em modo de tempo

# Configuração da cache L1 de instrução e dados
system.cpu.icache = create_cache(size='64kB', assoc=8)  # Tamanho e associatividade configuráveis
system.cpu.dcache = create_cache(size='64kB', assoc=8)  # Tamanho e associatividade configuráveis

# Configuração da cache L2
system.l2cache = create_cache(size='512kB', assoc=16)  # Tamanho e associatividade configuráveis

# Conexão das caches com a CPU
system.cpu.icache.connectCPU(system.cpu)
system.cpu.dcache.connectCPU(system.cpu)
system.l2cache.connectCPUSideBus(system.membus)

# Conexão das caches com a memória
system.cpu.icache.connectMemSideBus(system.membus)
system.cpu.dcache.connectMemSideBus(system.membus)
system.l2cache.connectMemSideBus(system.membus)

system.cpu.createInterruptController()  # Controlador de interrupção da CPU

# Configuração do sistema Ruby de memória
system.ruby = RubySystem()
system.ruby.clk_domain = system.clk_domain
system.ruby.number_of_virtual_networks = 3  # Número de redes virtuais
system.ruby.number_of_sequencers = 1  # Número de sequenciadores

# Configuração do protocolo de coerência de cache para MOESI_CMP_directory
system.ruby.coherent_ht = True
system.ruby.memory = SingleChannelDDR3_1600(size="3GB")

# Configuração do sistema operacional
system.kernel = Resource("x86-linux-kernel-4.4.186")  # Caminho para o kernel Linux
system.disk_image = DISK_PATH  # Imagem do disco

system.cpu = O3CPU()

# Configuração do ambiente
root = Root(full_system=True, system=system)
m5.instantiate()

# Execução da simulação
print("Starting simulation...")
print("Running command...")

command = "echo 'Executing monte carlo parallel.';" \
        + "cd ../home/gem5/pi-estimation;" \
        + "g++ -fopenmp monte-carlo-parallel-2000.cpp -o monte-carlo-parallel;" \
        + "./monte-carlo-parallel;" \
        + "sleep 10;" \
        + "m5 exit;"
m5.system.cpu.workload.run_command(command)

# Finalização da simulação
print('Exiting @ tick %d because %s' % (m5.curTick(), exit_event.getCause()))

