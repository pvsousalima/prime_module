Prime Module
============


Um modulo de Kernel para cálculo de números primos
--------------------------------------------------

Utilização
----------

#### Build:

	make

#### Clean:

	make clean

#### Install:

	sudo insmod primo.ko

#### Remove:

	sudo rmmod primo


Para verificar logs de impressão
--------------------------------

#### Opção 1:

	dmesg

#### Opção 2 (dinâmica):

	tail -f /var/log/kern.log


Aplicação usuário para acesso ao módulo
---------------------------------------

#### Build:

	gcc user.c -o user

#### Run:

	sudo ./user

Casos de Teste
--------------
- Kernel version 3.13.0-24-generic

- GCC (GNU C Compiler)

- Linux Ubuntu:

	Distributor ID:	Ubuntu
	Description:	Ubuntu 14.04.1 LTS
	Release:	14.04
	Codename:	trusty
