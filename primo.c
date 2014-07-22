/* Necessary includes for device drivers */
#include <linux/module.h> /* modulo de kernel */
#include <linux/kernel.h> /* printk() */
#include <linux/slab.h> /* kmalloc() */
#include <linux/types.h> /* size_t types */
#include <linux/proc_fs.h> /* /proc filesystem*/
#include <linux/miscdevice.h> /* /dev */
#include <linux/seq_file.h> /* sequence files */
#include <asm/uaccess.h> /* copy_from_user e copy_to_user */

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Pedro Victor de Sousa Lima");
MODULE_DESCRIPTION("Este módulo cria um device em /dev/primo e uma entrada \
  em /proc/primo. Recebe como entrada no /dev/primo um valor i e encontra o \
  i-esimo numero primo referente a este valor. Imprimindo o i-ésimo primo no \
  log do kernel e imprimindo uma lista de números primos obtidos até o i-ésimo \
  primo na entrada /proc/primo");


/* Global variables of the driver */

/* Major number */
int primo_major = 60;

/* Buffer to store data */
char *primo_buffer;

/* Valor a ser calculado */
long iesimo_primo = 0, last = 0;

/* Ponteiro para /proc */
struct proc_dir_entry *proc_file_entry;

/* Funcao de calculo do i-esimo primo */
int primo_calculate(struct seq_file *m){
   int i = 3, count, c;
   if (iesimo_primo == 1) {
      printk("The %ld prime number is: ", iesimo_primo);
      last = 2;
      seq_printf(m, "%ld\n", last);
      printk("2\n");
      return 0;
   }else{      
    printk("The %ld prime number is: ", iesimo_primo);
    seq_printf(m, "2\n");
    for (count = 2 ; count <= iesimo_primo;){
      for ( c = 2 ; c <= i - 1 ; c++ ){
         if ( i%c == 0 )
            break;
      }
      if ( c == i ) {
        seq_printf(m, "%d\n", i);
        last = i;
        count++;
      }
      i++;
    }
    printk("%ld\n",last);
   }
   return 0;
}

/* Inicializa uma estrutura de dados buffer no lado do kernel para receber dados */
int primo_open(struct inode *inode, struct file *filp) {

  /* Aloca memoria para o buffer de kernel */
    primo_buffer = kzalloc(20, GFP_KERNEL); 

    return 0;
}

/* Release do buffer */
int primo_release(struct inode *inode, struct file *filp) {

  /* Libera memoria do buffer de kernel */
  kfree(primo_buffer);

    return 0;
}

/* Leitura de dados do /dev nao permitida */
ssize_t primo_read(struct file *filp, char *buf, size_t count, loff_t *f_pos) { 

    /* Lida do /dev nao permitida */ 
    printk("Read from /dev not allowed\n");

    return 0; 
}

/*Escrita de dados do usuario no /dev/primo */
ssize_t primo_write(struct file *filp, const char *buf, size_t count, loff_t *f_pos){
    
    /* Transfere dados do user para o buffer do kernel */
    copy_from_user(primo_buffer, buf, count);
    
    /* Conversao da entrada para long */
    kstrtol(primo_buffer, 0, &iesimo_primo);

    return count;
}

/* Estrutura relativa as operacoes de /dev */
struct file_operations dev_fops = {
    .read = primo_read,
    .write = primo_write,
    .open = primo_open,
    .release = primo_release
};

/* Operações de leitura para /proc/primo */
 int proc_read(struct seq_file *m, void *v){

  /* Calcula o primo */
  primo_calculate(m);

  return 0;
}

/* Funcao de abertura de /proc */
int proc_open(struct inode *inode, struct file *file){
     return single_open(file, proc_read, NULL);
 }

/* Estrutura relativa a operacoes de /proc */
 struct file_operations proc_file_fops = {
     .open  = proc_open,
     .read  = seq_read,
     .llseek  = seq_lseek
 };

/* Funcao init */
int primo_init(void) {

  /* Retorno do registro do /dev */
    int primo_dev=0;

    /* Mensagem de insercao do modulo */
  printk("Inserting primo module\n");

  /* Registro do /dev */
    primo_dev = register_chrdev(primo_major, "primo", &dev_fops);
    if (primo_dev < 0) {
        printk("memory: cannot obtain major number %d\n", primo_major);
        return primo_dev;
    }

    /* Registro do /proc */
    proc_file_entry = proc_create("primo", 0, NULL, &proc_file_fops);
    if(proc_file_entry == NULL){
      printk("deu caca");
    }

    return 0;
}

/* Funcao exit */
void primo_exit(void) {

  /* Remocao de dispositivo /dev */
    unregister_chrdev(primo_major, "primo");

    /* Remocao de entrada /proc */
    proc_remove(proc_file_entry);

    /* Mensagem de remocao do modulo */
    printk("Removing primo module\n");
}


/* Declaration of the init and exit functions */
module_init(primo_init);
module_exit(primo_exit);
