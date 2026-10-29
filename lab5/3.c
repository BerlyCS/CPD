// productores y consumidores

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

double now_sec(void) {
  struct timespec ts;
  clock_gettime(CLOCK_MONOTONIC, &ts);
  return (double)ts.tv_sec + (double)ts.tv_nsec / 1e9;
}

int* buffer;
int cap;
int in = 0;
int out = 0;
int count = 0;

long producidos = 0;
long consumidos = 0;
long duplicados = 0;
long marcados = 0;
long suma_consumida = 0;
long total;
long items_por_productor;

char* vistos;              // vistos[i] = 1 si el valor i ya salio del buffer
int fin_produccion = 0;

pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t no_lleno = PTHREAD_COND_INITIALIZER;
pthread_cond_t no_vacio = PTHREAD_COND_INITIALIZER;

void* productor(void* arg) {
  (void)arg;
  for (long k = 0; k < items_por_productor; k++) {
    pthread_mutex_lock(&mtx);

    while (count == cap) {
      pthread_cond_wait(&no_lleno, &mtx);
    }

    int valor = (int)producidos;
    buffer[in] = valor;
    in = (in + 1) % cap;
    count++;
    producidos++;

    pthread_cond_signal(&no_vacio);
    pthread_mutex_unlock(&mtx);
  }
  return NULL;
}

void* consumidor(void* arg) {
  (void)arg;
  for (;;) {
    pthread_mutex_lock(&mtx);

    while (count == 0 && !fin_produccion) {
      pthread_cond_wait(&no_vacio, &mtx);
    }

    if (count == 0 && fin_produccion) {
      pthread_mutex_unlock(&mtx);
      break;
    }

    int valor = buffer[out];
    out = (out + 1) % cap;
    count--;
    consumidos++;
    suma_consumida += valor;

    if (vistos[valor]) {
      duplicados++;
    } else {
      vistos[valor] = 1;
      marcados++;
    }

    pthread_cond_signal(&no_lleno);
    pthread_mutex_unlock(&mtx);
  }
  return NULL;
}

int main(int argc, char** argv) {
  int nprod = 2;
  int ncons = 2;
  items_por_productor = 100000;
  cap = 8;

  if (argc > 1) nprod = atoi(argv[1]);
  if (argc > 2) ncons = atoi(argv[2]);
  if (argc > 3) items_por_productor = atol(argv[3]);
  if (argc > 4) cap = atoi(argv[4]);

  if (nprod < 1 || ncons < 1 || items_por_productor < 1 || cap < 1) {
    fprintf(stderr, "Uso: %s [productores] [consumidores] [items_por_productor] [capacidad]\n", argv[0]);
    return 1;
  }

  total = nprod * items_por_productor;

  buffer = (int*)malloc(cap * sizeof(int));
  vistos = (char*)calloc(total, sizeof(char));
  if (buffer == NULL || vistos == NULL) {
    fprintf(stderr, "malloc/calloc fallo\n");
    return 1;
  }

  pthread_t* ph = (pthread_t*)malloc(nprod * sizeof(pthread_t));
  pthread_t* ch = (pthread_t*)malloc(ncons * sizeof(pthread_t));
  if (ph == NULL || ch == NULL) {
    fprintf(stderr, "malloc fallo para hilos\n");
    return 1;
  }

  double t0 = now_sec();

  for (int i = 0; i < nprod; i++) {
    pthread_create(&ph[i], NULL, productor, NULL);
  }
  for (int i = 0; i < ncons; i++) {
    pthread_create(&ch[i], NULL, consumidor, NULL);
  }

  // esperar a que terminen los productores y avisar a los consumidores
  for (int i = 0; i < nprod; i++) {
    pthread_join(ph[i], NULL);
  }

  pthread_mutex_lock(&mtx);
  fin_produccion = 1;
  pthread_cond_broadcast(&no_vacio);
  pthread_mutex_unlock(&mtx);

  // los consumidores drenan lo que quede en el buffer y terminan
  for (int i = 0; i < ncons; i++) {
    pthread_join(ch[i], NULL);
  }

  double t = now_sec() - t0;

  long suma_esperada = total * (total - 1) / 2;

  printf("Productores: %d\n", nprod);
  printf("Consumidores: %d\n", ncons);
  printf("Items por productor: %ld\n", items_por_productor);
  printf("Capacidad del buffer: %d\n", cap);
  printf("Tiempo: %.6f s\n", t);
  printf("Producidos: %ld\n", producidos);
  printf("Consumidos: %ld\n", consumidos);
  printf("Marcados:   %ld\n", marcados);
  printf("Duplicados: %ld\n", duplicados);
  printf("Suma consumida: %ld (esperada %ld)\n", suma_consumida, suma_esperada);

  if (producidos == total && consumidos == total && marcados == total &&
      duplicados == 0 && suma_consumida == suma_esperada) {
    printf("Resultado: OK\n");
  } else {
    printf("Resultado: ERROR\n");
  }

  free(ch);
  free(ph);
  free(vistos);
  free(buffer);
  return 0;
}
