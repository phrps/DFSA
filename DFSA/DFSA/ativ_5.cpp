//#include <bits/stdc++.h>

#include <iostream>
#include <algorithm>
#include <fstream>
#include <limits>
#include <string>
#include <vector>
#include <list>
#include <cstring>
#include <cstdlib>
#include <cmath>
#include <cerrno>
#include <random>
#include <time.h>

#define LOWER_BOUND 0
#define EON_LEE 1
#define FAST_Q 2
#define ADAP_Q 3
#define CHEN 4
#define VAHEDI 5

typedef long long ll;

using namespace std;

int slot[500000], cur_iter;
double e_threshold = 1e-3;

long long factorial(long long x){
  if(x == 1) return 1;
  else return x * factorial(x - 1);
}

pair<int, pair<int, int> > Simulate(int ini_tags, int ini_frame, int num_rep, int param_q, int op_protocol, int *count_chen, int *count_vahedi) {
  pair<int, pair<int, int> > all_empty_col = make_pair(0, make_pair(0, 0));
  int identified, collided, nxt_frame, remain_tags;
  int cur_tag, cur_identified, q_first_iter, q_iter;
  double beta = numeric_limits<double>::max();
  double gama0, gama1 = 2.0, temp_exp, qfp = param_q, c = 0.2, ccoll = 0.21183, cidle = 0.15;
  bool q_identified;

  for (int i = 0; i < num_rep; ++i) {
    identified = 0;
    q_identified = false;

    if (op_protocol == FAST_Q || op_protocol == ADAP_Q)
      nxt_frame = (int)pow(2, param_q);
    else
      nxt_frame = ini_frame;

    while (identified < ini_tags) {
      remain_tags = ini_tags - identified;
      cur_iter += 2;
      collided = cur_identified = 0;

      if (!q_identified) {
        for (int j = 0; j < remain_tags; ++j) {
          cur_tag = rand() % nxt_frame;

          if (slot[cur_tag] < cur_iter - 1)
            slot[cur_tag] = cur_iter - 1;
          else
            slot[cur_tag] = cur_iter;
        }

        all_empty_col.first += nxt_frame;

        for (int j = 0; j < nxt_frame; ++j) {
          if (slot[j] < cur_iter - 1)
            all_empty_col.second.first++;
          else if (slot[j] == cur_iter)
            collided++;
          else
            identified++, cur_identified++;
        }

        all_empty_col.second.second += collided;
      }

      if (op_protocol == LOWER_BOUND) {
        nxt_frame = 2 * collided;
      } else if (op_protocol == EON_LEE) {
        do {
          gama0 = gama1;
          beta = nxt_frame / (gama0*collided + cur_identified);
          temp_exp = exp(-1.0 / beta);
          gama1 = (1.0 - temp_exp) / (beta*(1.0 - ((1.0 + (1.0 / beta)) * temp_exp)));
        } while (abs(gama0 - gama1) >= e_threshold);
        nxt_frame = (int) round(gama1*collided);
      } else if (op_protocol == ADAP_Q) {
        if (slot[0] < cur_iter - 1)
          qfp = std::max(0.0, qfp - c);
        else if (slot[0] == cur_iter)
          qfp = min(15.0, qfp + c);

        nxt_frame = (int) pow(2, (int) qfp);
      } else if (op_protocol == FAST_Q) {
        if (q_identified) {
          if (slot[0 + q_iter] < q_first_iter - 1) {
            qfp = std::max(0.0, qfp - cidle);
            q_identified = false;
          } else if (slot[0 + q_iter] == q_first_iter) {
            qfp = min(15.0, qfp + ccoll);
            q_identified = false;
          } else {
            q_iter++;
          }
        } else {
          if (slot[0] < cur_iter - 1) {
            qfp = std::max(0.0, qfp - cidle);
          } else if (slot[0] == cur_iter) {
            qfp = min(15.0, qfp + ccoll);
          } else {
            q_first_iter = cur_iter;
            q_identified = true;
            q_iter = 1;
          }
        }

        if (!q_identified)
          nxt_frame = (int)pow(2, (int)qfp);
      } else if(op_protocol == CHEN) {
          long long C = (long long)collided, S = (long long)cur_identified;
          long long E = (long long)nxt_frame - (long long)C - (long long)S;
          long long L = (long long)nxt_frame;
          long long n = (long long)S + 2 * (long long)C;
          double next = 0, previous = -1;

          while(previous < next){
            double pe = pow(1 - (((double)1)/L), n);
            double ps = (((double)n)/L) * pow(1 - (((double)1)/L), n - 1);
            double pc = 1 - pe - ps;
            previous = next;
            long long biggest = max(C, max(E, S));
            long long remain_1, remain_2;

            if(biggest == C)
              remain_1 = E, remain_2 = S;
            else if(biggest == E)
              remain_1 = C, remain_2 = S;
            else
              remain_1 = C, remain_2 = E;

            next = 1;
            for(long long i = biggest + 1; i <= L; ++i){
              if(remain_1 > 0){
                next *= double(i) / remain_1;
                remain_1--;
              } else if (remain_2 > 0){
                next *= double(i) / remain_2;
                remain_2--;
              } else
                printf("merda\n");
            }

            next *= pow(pe, E);
            next *= pow(ps, S);
            next *= pow(pc, C);
            n++;
          }
          ++(*count_chen);

          long long nchen = n - 2;
          nxt_frame = (int)nchen;
      } else if(op_protocol == VAHEDI){
        long long C = (long long)collided, S = (long long)cur_identified;
        long long E = (long long)nxt_frame - (long long)C - (long long)S;
        long long L = (long long)nxt_frame;
        long long n = (long long)S + 2 * (long long)C;
        double next = 0, previous = -1;
        double remain_1 = min(S, n - S);

        while(previous < next){
          double p1 = pow((1 - (double)E/L), n);
          double p2 = 1;

          for(long long i = max(S, n - S) + 1; i <= n; ++i){
            p2 *= (double)i / remain_1;
            remain_1--;
          }

          p2 *= (pow(L - E - S, n - S)) / (pow(L - E, n));
          remain_1 = S;

          while(remain_1 > 1){
            p2 *= remain_1;
            remain_1--;
          }

          double p3 = 0;

          for(long long k = 0; k <= C; ++k){
            for(long long v = 0; v <= C - k; ++v){
              ll m1 = min(k, C - k);
              double res1 = 1;

              for(ll x = max(k, C- k) + 1; x <= C; ++x){
                res1 *= (double)x / m1;
                m1--;
              }

              ll m2 = min(C - k - v, v);
              double res2 = 1;

              for(ll x = max(C - k - v, v) + 1; x <= C - k; ++x){
                res2 *= (double)x / m2;
                m2--;
              }

              ll m3 = n - S, m4 = n - S - k;
              double res3 = 1;

              while(m3 > 0 && m4 > 0){
                res3 *= (double) m3 / m4;
                m3--, m4--;
              }

              while(m3 > 0){
                res3 *= (double)m3;
                m3--;
              }

              double res4 = (pow(C - k - v, n - S - k) / pow(C, n - S));

              p3 += 1 + pow(-1, k + v) * res1 * res2 * res3 * res4;
              ++(*count_vahedi);
            }
          }

          previous = next;

          long long biggest = max(C, max(E, S));
          long long remain_1, remain_2;

          if(biggest == C)
            remain_1 = E, remain_2 = S;
          else if(biggest == E)
            remain_1 = C, remain_2 = S;
          else
            remain_1 = C, remain_2 = E;

          next = 1;
          for(long long i = biggest + 1; i <= L; ++i){
            if(remain_1 > 0){
              next *= double(i) / remain_1;
              remain_1--;
            } else if (remain_2 > 0){
              next *= double(i) / remain_2;
              remain_2--;
            } else
              printf("merda\n");
          }

          next *= p1 * p2 * p3;
          n += 1;
        }

        ll nvahedi = n - 2;
        nxt_frame = (int)nvahedi;
      }

      if (nxt_frame <= 0)
        nxt_frame = ini_frame;
    }
  }

  int avrg_all = all_empty_col.first / num_rep;
  int avrg_empty = all_empty_col.second.first / num_rep;
  int avrg_col = all_empty_col.second.second / num_rep;
  all_empty_col = make_pair(avrg_all, make_pair(avrg_empty, avrg_col));

  return all_empty_col;
}

int main() {

  FILE* fLower = fopen("lower.out","w");
  if (fLower == NULL)
  {
    printf("File Lower was not opened\n");
    return -1;
  }
  FILE* fLee = fopen("lee.out","w");
  if (fLee == NULL)
  {
    printf("File Lee was not opened\n");
    return -2;
  }
  FILE* fFastQ = fopen("fast_q.out","w");
  if (fFastQ == NULL)
  {
    printf("File FAST_Q was not opened\n");
    return -3;
  }

  FILE* fAdapQ = fopen("adap_q.out","w");
  if (fAdapQ == NULL)
  {
    printf("File ADAP_Q was not opened\n");
    return -4;
  }

  FILE* fChen = fopen("chen.out","w");
  if (fChen == NULL)
  {
    printf("File CHEN was not opened\n");
    return -5;
  }

  FILE* fVahedi = fopen("vahedi.out","w");
  if (fVahedi == NULL)
  {
    printf("File VAHEDI was not opened\n");
    return -6;
  }

  int num_protocols;
  int ini_tags = 100, max_tags = 1000;
  int inc_step = 100, num_rep = 2000, ini_frame = 64;
  pair<int, pair<int, int> > all_empty_col;

  srand(time(NULL));
  printf("Loading...\n");
  clock_t start = 0;
  double temp;
  int count_vahedi=0,count_chen=0;
  for (; ini_tags <= max_tags; ini_tags += inc_step) {
    start = clock();
    all_empty_col = Simulate(ini_tags, ini_frame, num_rep, 0, LOWER_BOUND,NULL,NULL);
    temp = ((double)(clock()-start) / CLOCKS_PER_SEC)*1000;
    fprintf(fLower, "%d %d %d %d %lf\n", ini_tags, all_empty_col.first, all_empty_col.second.first, all_empty_col.second.second, temp);
    start = clock();
    all_empty_col = Simulate(ini_tags, ini_frame, num_rep, 0, EON_LEE,NULL,NULL);
    temp = ((double)(clock()-start) / CLOCKS_PER_SEC)*1000;
    fprintf(fLee, "%d %d %d %d %lf\n", ini_tags, all_empty_col.first, all_empty_col.second.first, all_empty_col.second.second, temp);
    start = clock();
    all_empty_col = Simulate(ini_tags, ini_frame, num_rep, 4, FAST_Q,NULL,NULL);
    temp = ((double)(clock()-start) / CLOCKS_PER_SEC)*1000;
    fprintf(fFastQ, "%d %d %d %d %lf\n", ini_tags, all_empty_col.first, all_empty_col.second.first, all_empty_col.second.second, temp);
    start = clock();
    all_empty_col = Simulate(ini_tags, ini_frame, num_rep, 4, ADAP_Q,NULL,NULL);
    temp = ((double)(clock()-start) / CLOCKS_PER_SEC)*1000;
    fprintf(fAdapQ, "%d %d %d %d %lf\n", ini_tags, all_empty_col.first, all_empty_col.second.first, all_empty_col.second.second, temp);
    start = clock();
    all_empty_col = Simulate(ini_tags, ini_frame, num_rep, 4, CHEN,&count_chen,&count_vahedi);
    temp = ((double)(clock()-start) / CLOCKS_PER_SEC)*1000;
    fprintf(fChen, "%d %d %d %d %lf %d\n", ini_tags, all_empty_col.first, all_empty_col.second.first, all_empty_col.second.second, temp,count_chen);
    start = clock();
    all_empty_col = Simulate(ini_tags, ini_frame, num_rep, 4, VAHEDI,&count_chen,&count_vahedi);
    temp = ((double)(clock()-start) / CLOCKS_PER_SEC)*1000;
    fprintf(fVahedi, "%d %d %d %d %lf %d\n", ini_tags, all_empty_col.first, all_empty_col.second.first, all_empty_col.second.second, temp,count_vahedi);
    printf("Tags: %4d, All: %5d, Empty: %5d, Collision: %5d\n", ini_tags, all_empty_col.first, all_empty_col.second.first, all_empty_col.second.second);
    count_vahedi=count_chen=0;

  }
  fclose(fLower);
  fclose(fLee);
  fclose(fFastQ);
  fclose(fAdapQ);
  fclose(fChen);
  fclose(fVahedi);
  printf("Done!\n");
  printf("Plotting graphics\n");
  system("python plot.py");
  return 0;
}
