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
#include <random>
#include <time.h>

#define LOWER_BOUND 0
#define EON_LEE 1
#define ALGO_Q 2

using namespace std;

int slot[500000], cur_iter;
double e_threshold = 1e-3;

pair<int, pair<int, int> > Simulate(int ini_tags, int ini_frame, int num_rep, int param_q, int op_protocol) {
  pair<int, pair<int, int> > all_empty_col = make_pair(0, make_pair(0, 0));
  int identified, collided, nxt_frame, remain_tags;
  int cur_tag, cur_identified;
  double beta = numeric_limits<double>::max();
  double gama0, gama1 = 2.0, temp_exp, qfp = param_q, c = 0.2;

  for (int i = 0; i < num_rep; ++i) {
    identified = 0;

    if (op_protocol != ALGO_Q)
      nxt_frame = ini_frame;
    else
      nxt_frame = (int) pow(2, param_q);

    while (identified < ini_tags) {
      remain_tags = ini_tags - identified;
      cur_iter += 2;
      collided = cur_identified = 0;

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

      if (op_protocol == LOWER_BOUND)
        nxt_frame = 2 * collided;
      else if (op_protocol == EON_LEE) {
        do {
          gama0 = gama1;
          beta = nxt_frame / (gama0*collided + cur_identified);
          temp_exp = exp(-1.0 / beta);
          gama1 = (1.0 - temp_exp) / (beta*(1.0 - ((1.0 + (1.0 / beta)) * temp_exp)));
        } while (abs(gama0 - gama1) >= e_threshold);
        nxt_frame = (int) round(gama1*collided);
      } else if (op_protocol == ALGO_Q){
        if (slot[0] < cur_iter - 1)
          qfp = std::max(0.0, qfp - c);
        else if (slot[0] == cur_iter)
          qfp = min(15.0, qfp + c);

        nxt_frame = (int) pow(2, (int) qfp);
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
  errno_t err;
  FILE* fLower;
  if ((err = fopen_s(&fLower, "lower.out", "w")) != 0) {
    printf("File Lower was not opened\n");
    return -1;
  }
  FILE* fLee;
  if ((err = fopen_s(&fLee, "lee.out", "w")) != 0) {
    printf("File Lee was not opened\n");
    return -2;
  }
  FILE* fQ;
  if ((err = fopen_s(&fQ, "q.out", "w")) != 0) {
    printf("File Q was not opened\n");
    return -3;
  }

  int num_protocols;
  int ini_tags = 100, max_tags = 1000;
  int inc_step = 100, num_rep = 2000, ini_frame = 64;
  pair<int, pair<int, int> > all_empty_col;

  srand(time(NULL));
  printf("Loading...\n");
  double temp = 0;
  for (; ini_tags <= max_tags; ini_tags += inc_step) {
    all_empty_col = Simulate(ini_tags, ini_frame, num_rep, 0, LOWER_BOUND);
    temp = clock() / (double) cur_iter;
    fprintf(fLower, "%d %d %d %d %lf\n", ini_tags, all_empty_col.first, all_empty_col.second.first, all_empty_col.second.second, temp);
    all_empty_col = Simulate(ini_tags, ini_frame, num_rep, 0, EON_LEE);
    temp = clock() / (double) cur_iter;
    fprintf(fLee, "%d %d %d %d %lf\n", ini_tags, all_empty_col.first, all_empty_col.second.first, all_empty_col.second.second, temp);
    all_empty_col = Simulate(ini_tags, ini_frame, num_rep, 4, ALGO_Q);
    temp = clock() / (double) cur_iter;
    fprintf(fQ, "%d %d %d %d %lf\n", ini_tags, all_empty_col.first, all_empty_col.second.first, all_empty_col.second.second, temp);
    printf("Tags: %4d, All: %5d, Empty: %5d, Collision: %5d\n", ini_tags, all_empty_col.first, all_empty_col.second.first, all_empty_col.second.second);
  }
  fclose(fLower);
  fclose(fLee);
  fclose(fQ);
  printf("Done!\n");
  printf("Plotting graphics\n");
  system("python plot.py");
  return 0;
}
