// Author: Lin
// Date: 2025-12-21 15:03:48
// Description:

#include "src/states.hpp"
#include <QDebug>
#include <QFile>
#include <QTextStream>

const QMap<QString, int> States::names2i={
  {"基础攻击力", 0}, {"额外攻击力", 1}, {"基础生命值", 2}, {"额外生命值", 3}, {"基础防御力", 4},
  {"额外防御力", 5}, {"攻击力百分比", 6}, {"生命值百分比", 7}, {"防御力百分比", 8}, {"暴击率", 9},
  {"暴击伤害", 10}, {"元素充能效率", 11}, {"元素精通", 12}, {"反应系数提升", 13},
  {"攻击倍率", 14}, {"生命倍率", 15}, {"防御倍率", 16}, {"额外倍率", 17},
  {"风元素伤害加成", 18}, {"火元素伤害加成", 19}, {"水元素伤害加成", 20},
  {"岩元素伤害加成", 21}, {"雷元素伤害加成", 22}, {"草元素伤害加成", 23},
  {"冰元素伤害加成", 24}, {"伤害加成", 25}, {"降低防御", 26}, {"无视防御", 27},
  {"元素抗性", 28}
};

const QVector<QString> States::names={
  "基础攻击力", "额外攻击力", "基础生命值", "额外生命值", "基础防御力",
  "额外防御力", "攻击力百分比", "生命值百分比", "防御力百分比", "暴击率",
  "暴击伤害", "元素充能效率", "元素精通", "反应系数提升",
  "攻击倍率", "生命倍率", "防御倍率", "额外倍率",
  "风元素伤害加成", "火元素伤害加成", "水元素伤害加成",
  "岩元素伤害加成", "雷元素伤害加成", "草元素伤害加成",
  "冰元素伤害加成", "伤害加成", "降低防御", "无视防御",
  "元素抗性"
};

/*
    0   1   2   3   4   5   6  7  8  9   10    11    12  13    14     15     16
  ATK ATK% HP HP% DEF DEF% CR CD EM ER Anemo Pyro Hydro Geo Electro Dendro Cryo
*/
States::States(QObject *parent): 
  QObject(parent), 
  MainStates{0b0100, 0b01, 0b010, 0b010, 0b010}, 
  SubStates(5,0), 
  MainMasks{0b0100, 0b01, 0b01100101010, 0b011111110100101010, 0b0111101010}, 
  SubMasks{0b01111111011, 0b01111111110, 0b01111111111, 0b01111111111, 0b01111111111}, 
  states{
    2,3,6,7,8,
    0,1,6,7,8,
    3,2,6,7,8,
    12,3,6,7,8,
    7,2,3,6,8
  },
  buffDatas{29, {}}
{
  restState();
}

uT States::relicState(int i) const {
  // it is the main stat
  if( i%5==0 ){
    return ~(MainStates[i/5] | SubStates[i/5]) & MainMasks[i/5];
  } else {
    return ~(MainStates[i/5] | SubStates[i/5]) & SubMasks[i/5];
  }
}

void States::setRelicState(int i, uT v) {
  // first check which stat is updated
  int idx=i/5;
  // recalculate MainStates or SubStates
  if(i%5==0){
    MainStates[idx]=(1<<v);
  }else {
    SubStates[idx]=(SubStates[idx]&~(1<<states[i]))|(1<<v);
  }
  // update the states
  states[i]=v;
}

uT States::getState(int i) {
  return states[i];
}

void States::restState(){
  // reset MainState
  for(int i=0; i<5; ++i)MainStates[i]=(1<<states[5*i]);
  // reset SubState
  for(int i=0; i<5; ++i){
    SubStates[i]=0;
    for(int j=1; j<5; ++j){
      SubStates[i]|=(1<<states[5*i+j]);
    }
    // qDebug() << SubStates[i];
  }
}

uT States::getBufferSize(int i) const {
  return buffDatas[i].size();
}

QVariant States::getBufferData(int i, int j){
  if(i<0||i>=buffDatas.size()||j<0||j>=buffDatas[i].size()){
    return QVariant();
  }
  auto p=buffDatas[i][j];
  QVariantMap qmap;
  qmap.insert("tag", p.first);
  qmap.insert("value", p.second);
  return QVariant(qmap);
}

QVariant States::getBuffer(int i){
  if(i<0||i>=buffDatas.size()){
    return QVariant();
  }
  QVariantList list;
  for(auto &[name, value]: buffDatas[i]){
    QVariantMap qmap;
    qmap.insert("tag", name);
    qmap.insert("value", QString::number(value));
    list.append(QVariant(qmap));
  }
  return QVariant(list);
}

void States::saveYaml(QString path){
  qDebug() << "Saving YAML file to " << path;
  if(path.isEmpty()){
    qDebug() << "Invalid path!";
    return;
  }
  updateBuffer();
  qDebug() << buffDatas;
  YAML::Node root;
  
  // save Buffer
  for(int i=0; i<buffDatas.size(); ++i){
    for(auto &[name, value]: buffDatas[i]){
      root["Buffer"][names[i].toStdString()][name.toStdString()]=value;
    }
  }
  qDebug() << buffDatas;
  // save Relic
  root["Relic"].SetStyle(YAML::EmitterStyle::Flow);
  for(int i=0; i<states.size(); ++i){
    root["Relic"].push_back(states[i]);
  }

  // std::ofstream fout(path.toStdString());
  // fout << root;
  // fout.close();
  
  QFile file(path);
  if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
    qDebug() << "Failed to open file for writing:" << file.errorString();
    return;
  }
  QTextStream out(&file);
  std::string yamlContent=YAML::Dump(root);
  out << QString::fromStdString(yamlContent);

  file.close();
  qDebug() << "YAML file saved successfully!";
}

void States::loadYaml(QString path){
  YAML::Node config=YAML::LoadFile(path.toStdString());

  // load Buffer
    
  for(const auto &p: config["Buffer"]){
    QString key=QString::fromStdString(p.first.as<std::string>());
    int i=names2i[key];
    buffDatas[i].clear();
    YAML::Node subnode=p.second;
    for(const auto &q: subnode){
      QString tag=QString::fromStdString(q.first.as<std::string>());
      double value=q.second.as<double>();

      buffDatas[i].append(qMakePair(tag, value));
    }
  }
  qDebug() << buffDatas;
  
  loadBufferConfig();

  // load Relic
  for(int i=0; i<25; ++i){
    states[i]=config["Relic"][i].as<int>();
  }
  restState();
  loadRelicConfig();
}

void States::clearBufferData(int i){
  if(i<0||i>=buffDatas.size()) return;
  buffDatas[i].clear();
}

void States::updateBufferData(int i, QString tag, double value){
  if(i<0||i>=buffDatas.size()) return;
  buffDatas[i].append(qMakePair(tag, value));
}

void States::test_generate(){
  // cal buffer total;
  emit updateBuffer();
  QVector<double> buffer_totals(29, 0.0);
  for(int i=0; i<buffDatas.size(); ++i){
    for(auto &[name, value]: buffDatas[i]){
      buffer_totals[i]+=value;
    }
  }
  emit sender(states, buffer_totals);
}

void States::receiveBuffer(int tagKey, QVector<QPair<QString, double>>data){
  if(tagKey<0||tagKey>=buffDatas.size()) return;
  buffDatas[tagKey]=data;
}