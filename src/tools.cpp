// Author: Lin
// Date: 2025-12-27 10:55:29
// Description:


#include "src/tools.hpp"
#include <QDebug>

using std::min;

vT<int> getSubStates(const vT<uT> &states){
  vT<int> fm(GiCal::MAX_COLUMN, 0);
  for(int i=0; i<25; ++i){
    if(i%5!=0){
      fm[states[i]]++;
    }
  }
  vT<int> fs;
  for(auto &v: fm){
    if(v>0)fs.append(v);
  }
  return fs;
}

vT<int> getSubIndexs(const vT<uT> &states){
  vT<int> fm(GiCal::MAX_COLUMN, 0);
  for(int i=0; i<25; ++i){
    if(i%5!=0){
      fm[states[i]]++;
    }
  }
  vT<int> fi;
  for(int i=0; i<fm.size(); ++i){
    if(fm[i]>0)fi.append(i);
  }
  return fi;
}

QVector<std::array<double, GiCal::MAX_COLUMN>> generate(const vT<uT> &ori_states, int m){
  vT<int> states=getSubStates(ori_states);
  qDebug() << "get states " << states;
  int n=states.size();
  vT<int> sups(n, 0);
  for(int i=0; i<n; ++i)sups[i]=states[i]*5;
  
  vT<int> path(n, 0);
  QVector<std::array<double, GiCal::MAX_COLUMN>> data;
  data.reserve(200000);

  auto dfs=[&](auto&& f, int state_i, int residual, vT<int> &path)->void{
    if(state_i==n){
      std::array<double, GiCal::MAX_COLUMN> row;
      row.fill(0);
      for(int j=0; j<n; ++j)row[j]=(double)path[j];
      data.append(row);
      return;
    }
    for (int v=0; v<=min(sups[state_i], residual); ++v) {
      path[state_i]=v;
      f(f, state_i+1, residual-v, path);
    }
  };

  dfs(dfs, 0, m, path);
  qDebug() << "generate finish";
  qDebug() << "total data size " << data.size();
  return data;
};

QVector<std::array<double, GiCal::MAX_COLUMN>> calculate(const vT<uT> &ori_states, const vT<double> &ori_buffers, int m){
  /*
    @ori_states: original relic states, size 25
    @buffers: buffer states, size 29
  */
  // TODO
  double lv1=90, lv2=100;
  vT<double> buffers{ori_buffers};
  vT<int> states=getSubStates(ori_states);
  vT<int> findexs=getSubIndexs(ori_states);
  int nstats=states.size();

  // update sub stat buffer
  for(int i=0, idx; i<nstats; ++i){
    idx=findexs[i];
    buffers[GiCal::relic2buffer[idx]]+=states[i]*GiCal::statvalues[idx];
  }
  // update main stat buffer
  for(int i=0, idx; i<ori_states.size(); i+=5){
    idx=ori_states[i];
    buffers[GiCal::relic2buffer[idx]]+=GiCal::fullvalues[idx];
  }

  // get all combinations
  QVector<std::array<double, GiCal::MAX_COLUMN>> data=generate(ori_states, m);
  int d=1;
  for(auto &row: data){
    QVector<double> buffer_copy{buffers};

    // update buffer copy with row
    for(int i=0, idx; i<nstats; ++i){
      idx=findexs[i];
      buffer_copy[GiCal::relic2buffer[idx]]+=row[i]*GiCal::statvalues[idx];
    }

    // calculate final damage
    // ATK, HP, DEF
    row[nstats] = buffer_copy[0]*(1+buffer_copy[6]/100.)+buffer_copy[1];
    row[nstats+1] = buffer_copy[2]*(1+buffer_copy[7]/100.)+buffer_copy[3];
    row[nstats+2] = buffer_copy[4]*(1+buffer_copy[8]/100.)+buffer_copy[5];

    // ATK%, HP%, DEF%
    row[nstats+3] = buffer_copy[6]/100.;
    row[nstats+4] = buffer_copy[7]/100.;
    row[nstats+5] = buffer_copy[8]/100.;

    // EM, ER
    row[nstats+6] = buffer_copy[12];
    row[nstats+7] = buffer_copy[11]/100.;

    // CR, CD
    row[nstats+8] = buffer_copy[9]/100.;
    row[nstats+9] = buffer_copy[10]/100.;

    // dmg delta
    row[nstats+10] = buffer_copy[25]/100.;

    // def multiplier
    row[nstats+11] = (lv1+100)/(lv1+100+(lv2+100)*(1-buffer_copy[26]/100.)*(1-buffer_copy[27]/100.));

    // react multiplier
    row[nstats+12] = 
      buffer_copy[28]<0. ? (1-buffer_copy[28]/200.) : (buffer_copy[28]<=75. ? 1-buffer_copy[28]/100. : (1/(1+buffer_copy[28]/25.)));

    // final dmg
    row[nstats+13] = 
      (row[nstats]*buffer_copy[14]/100.+row[nstats+1]*buffer_copy[15]/100.+row[nstats+2]*buffer_copy[16]/100.+buffer_copy[17])*
      (1+row[nstats+9])*
      (1+buffer_copy[13]/100.+2.78*row[nstats+6]/(1400+row[nstats+6]))*
      (1+row[nstats+10])*
      row[nstats+11]*
      row[nstats+12]*2;

    if(d>0){
      qDebug() << "ATK " << row[nstats];
      qDebug() << "HP " << row[nstats+1];
      qDebug() << "DEF " << row[nstats+2];
      qDebug() << "CD " << row[nstats+9];
      qDebug() << "EM " << buffer_copy[13]/100.+2.78*row[nstats+6]/(1400+row[nstats+6]);
      qDebug() << "dmg delta " << row[nstats+10];
      qDebug() << "def mul" << row[nstats+11];
      qDebug() << "react mul" << row[nstats+12];
      qDebug() << "final dmg " << row[nstats+13];
      --d;
    }
  }
  return data;
};