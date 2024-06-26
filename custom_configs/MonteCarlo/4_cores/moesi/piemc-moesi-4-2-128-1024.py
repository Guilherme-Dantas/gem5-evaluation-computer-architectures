"""
    Architecture configuration:
        - O3:
            -> 4 cores
            -> 3GHz
            -> Using KVM
            -> X86
        - Cache Hierarchy:
            -> MOESI
            -> Two Level
        - Memory:
            -> DDR3
            -> 1600MHz
            -> 3GB
        - Kernel:
            -> From Gem5 Resources
            -> Version 4.4.186
        - Workload:
            - Parallel pi estimation using Monte Carlo
"""
import os
from gem5.utils.requires import requires
from gem5.components.boards.x86_board import X86Board
from gem5.components.memory.single_channel import SingleChannelDDR3_1600
from gem5.components.processors.simple_processor import (
    SimpleProcessor,
)
from gem5.components.processors.cpu_types import CPUTypes
from gem5.isas import ISA
from gem5.coherence_protocol import CoherenceProtocol
from gem5.simulate.simulator import Simulator
from gem5.simulate.exit_event import ExitEvent
from gem5.resources.resource import DiskImageResource
from gem5.resources.resource import Resource

DISK_PATH = "/home/guilherme.dantas/disk-images/x86-ubuntu"

requires(
    isa_required=ISA.X86,
    coherence_protocol_required=CoherenceProtocol.MOESI_CMP_DIRECTORY,
    kvm_required=True,
)

from gem5.components.cachehierarchies.ruby.moesi_two_level_cache_hierarchy import (
    MOESITwoLevelCacheHierarchy,
)

cache_hierarchy = MOESITwoLevelCacheHierarchy(
    l1d_size="128kB",
    l1d_assoc=8,
    l1i_size="128kB",
    l1i_assoc=8,
    l2_size="1024kB",
    l2_assoc=16,
    num_l2_banks=1,
)

memory = SingleChannelDDR3_1600(size="3GB")

processor = SimpleProcessor(
    cpu_type=CPUTypes.O3,
    isa=ISA.X86,
    num_cores=4,
)

board = X86Board(
    clk_freq="3GHz",
    processor=processor,
    memory=memory,
    cache_hierarchy=cache_hierarchy,
)

command = "echo 'Executing monte carlo parallel.';" \
        + "cd ../home/gem5/pi-estimation;" \
        + "g++ -fopenmp monte-carlo-parallel-2000.cpp -o monte-carlo-parallel;" \
        + "./monte-carlo-parallel;" \
        + "sleep 10;" \
        + "m5 exit;"

board.set_kernel_disk_workload(
    kernel=Resource("x86-linux-kernel-4.4.186"),
    disk_image=DiskImageResource(
        local_path=DISK_PATH,
        root_partition="1"),
    readfile_contents=command,
)

simulator = Simulator(
    board=board,
)
simulator.run()
