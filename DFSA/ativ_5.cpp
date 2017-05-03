//#include <bits/stdc++.h>

#include <iostream>
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

using namespace std;

int slot[500000], cur_iter;
double e_threshold = 1e-3;

pair<int, pair<int, int> > Simulate(int ini_tags, int ini_frame, int num_rep, int op_protocol){
  pair<int, pair<int, int> > all_empty_col = make_pair(0, make_pair(0, 0));
  int identified, collided, nxt_frame, remain_tags;
  int cur_tag, cur_identified;
  double beta = numeric_limits<double>::max();
  double gama0, gama1 = 2.0, temp_exp;

  for(int i = 0; i < num_rep; ++i){
    identified = 0;
    nxt_frame = ini_frame;

    while(identified < ini_tags){
      remain_tags = ini_tags - identified;
      cur_iter += 2;
      collided = cur_identified = 0;

      for(int j = 0; j < remain_tags; ++j){
        cur_tag = rand() % nxt_frame;

        if(slot[cur_tag] < cur_iter - 1)
          slot[cur_tag] = cur_iter - 1;
        else
          slot[cur_tag] = cur_iter;
      }

      all_empty_col.first += nxt_frame;

    for(int j = 0; j < nxt_frame; ++j){
        if(slot[j] < cur_iter - 1)
          all_empty_col.second.first++;
        else if(slot[j] == cur_iter)
          collided++;
        else
          identified++, cur_identified++;
      }
      
      all_empty_col.second.second += collided;

      if(op_protocol == LOWER_BOUND)
        nxt_frame = 2 * collided;
      else if(op_protocol == EON_LEE){
        do {
          gama0 = gama1;
          beta = nxt_frame / (gama0*collided + cur_identified);
          temp_exp = exp(-1.0 / beta);
          gama1 = (1.0 - temp_exp) / (beta*(1.0 - ((1.0 + (1.0 / beta)) * temp_exp)));
        } while (abs(gama0 - gama1) >= e_threshold);
        nxt_frame = (int) round(gama1*collided);
      }
      if (nxt_frame <= 0)
        nxt_frame = ini_frame;
    }
  }

  int avrg_all = all_empty_col.first/num_rep;
  int avrg_empty = all_empty_col.second.first/num_rep;
  int avrg_col = all_empty_col.second.second/num_rep;
  all_empty_col = make_pair(avrg_all, make_pair(avrg_empty, avrg_col));

  return all_empty_col;
}

int main(){
  int num_protocols;
  int ini_tags = 100, max_tags = 1000;
  int inc_step = 100, num_rep = 2000, ini_frame = 64;
  pair<int, pair<int, int> > all_empty_col;

  srand(time(NULL));
  
  for(; ini_tags <= max_tags; ini_tags += inc_step){
    all_empty_col = Simulate(ini_tags, ini_frame, num_rep, EON_LEE);
    printf("Tags: %4d, All: %5d, Empty: %5d, Collision: %5d\n", ini_tags, all_empty_col.first, all_empty_col.second.first, all_empty_col.second.second);
  }
  return 0;
}
