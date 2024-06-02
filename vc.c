#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <getopt.h>

#define CIRCLE 360.0
#define SEMICIRCLE 180.0
#define ISAF ((amplitude > 0) && (phase >= 0) && (phase < CIRCLE))
#define ERMSG "Please type correct data"
#define ERMEM "Memory allocation error"
#define DIF "dynamic impact factor:"
#define CW "correction weight:    "
#define HEADERCALC "sub_amp_phase | div_amp_phase | add_amp_phase | mul_amp_phase"
#define HEADERBAL "                    weight | phase"

enum {two = 2, three = 3, first = 0, last = 1, weight = 2};

struct vibro {
  double amp;
  double ph;
  };

struct vibro *v[three];

void initcalc(int msv)
{
  double amplitude, phase;
  int i, n;
  for(i = first; i < msv; i++) {
    n = scanf("%lf %lf", &amplitude, &phase);
    if(ISAF && n) {
      v[i] = malloc(sizeof(struct vibro));
      if (v[i] == NULL) {
        puts(ERMEM);
        exit(1);
      }
      v[i]->amp = amplitude;
      v[i]->ph = phase;
    } else {
      puts(ERMSG);
      exit(2);
    }
  }
}

void printinit(int msv)
{
  int i;
  for(i = first; i < msv; i++) {
    printf("%.2f %.2f%c", v[i]->amp, v[i]->ph, i < msv-1 ? ' ' : '\n');
  }
}	

double mul_amp(double f_amp, double l_amp)
{
  return f_amp * l_amp;
}

double mul_ph(double f_ph, double l_ph)
{
  if((f_ph + l_ph) >= CIRCLE) {
    return fabs((CIRCLE - (f_ph + l_ph)));
  } else {
    return f_ph + l_ph;
  }
}

double div_amp(double f_amp, double l_amp)
{
  return f_amp / l_amp;
}

double div_ph(double f_ph, double l_ph)
{
  if(f_ph >= l_ph) {
    return f_ph - l_ph;
  } else {
    return CIRCLE - (l_ph - f_ph);
  }
}

double l_ph_for_sub(double f_ph, double l_ph)
{
  if(f_ph == l_ph) {
    return l_ph;
  }
  if(l_ph >= SEMICIRCLE) {
    return l_ph - SEMICIRCLE;
  } else {
    return l_ph + SEMICIRCLE;
  }
}

double delta(double f_ph, double l_ph)
{
  if((fabs(f_ph - l_ph)) > SEMICIRCLE) {
    return CIRCLE - fabs(f_ph - l_ph);
  } else {
    return fabs(f_ph - l_ph);
  }
}

double sub_amp(double f_amp, double f_ph, double l_amp, double l_ph)
{
  return sqrt((f_amp * f_amp + l_amp * l_amp) - (2 * f_amp * l_amp * 
    cos((delta(f_ph, l_ph) * (M_PI / SEMICIRCLE)))));
}

double add_amp(double f_amp, double f_ph, double l_amp, double l_ph)
{
  return sqrt(2 * ((f_amp * f_amp + l_amp * l_amp)) - 
    (sub_amp(f_amp, f_ph, l_amp, l_ph) * sub_amp(f_amp, f_ph, l_amp, l_ph)));
}

double sub_add_ph(double f_amp, double f_ph, double l_amp, double l_ph)
{
  double raz, sum, deg, degr, tmp_a, tmp_b, tmp_c, tmp_d, tmp_e, tmp_f;
  raz = sub_amp(f_amp, f_ph, l_amp, l_ph) / 2;
  sum = add_amp(f_amp, f_ph, l_amp, l_ph) / 2;
  deg = (SEMICIRCLE / M_PI) * (acos ((((sum * sum) + (f_amp * f_amp)) 
    - (raz * raz)) / (2 * sum * f_amp)));
  degr = delta(f_ph, l_ph) - deg;
  tmp_a = deg + f_ph;
  if(tmp_a > CIRCLE) {
    tmp_a = tmp_a - CIRCLE;
  }
  tmp_b = degr + l_ph;
  if(tmp_b > CIRCLE) {
    tmp_b = tmp_b - CIRCLE;
  }
  tmp_c = tmp_b;
  if(tmp_c > CIRCLE) {
    tmp_c = tmp_c - CIRCLE;
  }
  tmp_d = tmp_a;
  if(tmp_d > CIRCLE) {
    tmp_d = tmp_d - CIRCLE;
  }
  if((f_ph - l_ph) < 0) {
    tmp_e = tmp_a;
  } else {
    tmp_e = tmp_b;
  }
  if((f_ph - l_ph) < SEMICIRCLE) {
    tmp_f = tmp_c;
  } else {
    tmp_f = tmp_d;
  }
  if(fabs(f_ph - l_ph) < SEMICIRCLE) {
    return tmp_e;
  } else if(tmp_f == CIRCLE) {
    return 0;
  } else {
    return tmp_f;
  }
}

double sub_ph(double f_amp, double f_ph, double l_amp, double l_ph)
{
  if((f_amp == l_amp) && (f_ph == l_ph)) {
    return f_ph - l_ph;
  } else {
    return sub_add_ph(f_amp, f_ph, l_amp, l_ph);
  }
}

double add_ph(double f_amp, double f_ph, double l_amp, double l_ph)
{
  if((f_amp == l_amp) && (f_ph == l_ph)) {
    return f_ph;
  } else {
     return sub_add_ph(f_amp, f_ph, l_amp, l_ph);
  }
}
void printcalc()
{
  double temp;
  temp = sub_amp(v[first]->amp, v[first]->ph, v[last]->amp, v[last]->ph);
  printf("%.2f ", temp);
  temp = sub_ph(v[first]->amp, v[first]->ph, v[last]->amp, 
    l_ph_for_sub(v[first]->ph, v[last]->ph));
  printf("%.2f |", temp);
  temp = div_amp(v[first]->amp, v[last]->amp);
  printf(" %.2f ", temp);
  temp = div_ph(v[first]->ph, v[last]->ph);
  printf("%.2f |", temp);
  temp = add_amp(v[first]->amp, v[first]->ph, v[last]->amp, v[last]->ph);
  printf(" %.2f ", temp);
  temp = add_ph(v[first]->amp, v[first]->ph, v[last]->amp, v[last]->ph);
  printf("%.2f |", temp);
  temp = mul_amp(v[first]->amp, v[last]->amp);
  printf(" %.2f ", temp);
  temp = mul_ph(v[first]->ph, v[last]->ph);
  printf("%.2f\n", temp);
}

void printbal()
{
  double dif_amp, dif_ph, cw_amp, cw_ph;
  dif_amp = div_amp(sub_amp(v[first]->amp, v[first]->ph, v[last]->amp,
    v[last]->ph), v[weight]->amp); 
  dif_ph = div_ph(sub_ph(v[first]->amp, v[first]->ph, v[last]->amp, 
    v[last]->ph), v[weight]->ph);
  printf("%s %.2f %.2f\n", DIF, dif_amp, dif_ph);
  cw_amp = div_amp(v[first]->amp, dif_amp); 
  cw_ph = div_ph(v[first]->ph, dif_ph);
  printf("%s %.2f %.2f\n", CW, cw_amp, cw_ph);
}

void freev()
{
  free(v[three]);
}

int main(int argc, char **argv)
{
  if(argc<2) {
    initcalc(two);
    printinit(two);
    puts(HEADERCALC);
    printcalc();
  } else {
    int opt;
    while((opt = getopt(argc, argv, "bht")) != -1) {
      switch(opt) {
      case 'b':
        initcalc(three);
        printinit(three);
        puts(HEADERBAL);
        printbal();
        break;
      case 'h':
        system("less README.md");
        break;
      case 't':
        initcalc(two);
        printcalc();
        break;
      default:
        printf("Incorrect option\n");
        exit(EXIT_FAILURE);
      }
    }
  }
  freev();
  return 0;
}
