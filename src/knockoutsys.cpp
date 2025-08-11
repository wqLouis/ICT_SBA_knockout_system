#include "knockoutsys.h"
#include "ui_knockoutsys.h"

#include <QDebug>
#include <QTableWidget>
#include <QVector>
#include <cmath>
#include <QGraphicsTextItem>
#include <QGraphicsView>
#include <QString>
#include <random>
#include <ctime>

knockOutSys::knockOutSys(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::knockOutSys)
{
    ui->setupUi(this);

    scene = new QGraphicsScene(this);

    ui->graphicsView->setScene(scene);
}

knockOutSys::~knockOutSys()
{
    delete ui;
}

class participants
{
public:
    int id;
    QString name;
    QString school;
    bool isSeed;
    int group = -1;
};

using namespace std;

int getClosestPowerTo2(int x){
    return pow(2, ceil(log(x)/log(2)));
}

int seedRemaining(QVector<participants> list){
    int seedCount = 0;
    for (int i = 0; i < list.size(); i++){
        if (list[i].isSeed){
            seedCount ++;
        }
    }
    return seedCount;
}

int maxLen(QVector<participants> list){
    int maxl = 0;
    for (int i = 0; i < list.size(); i++){
        if (maxl < max(list[i].name.length()+5,list[i].school.length()+7)){
            maxl = max(list[i].name.length()+5,list[i].school.length()+7);
        }
    }
    return maxl;
}

bool isFromLargestSchoolGroup(QVector<participants> *list,QString school){
    QVector<QString> schoolList;
    schoolList.append(school);
    for(int i = 0; i < list->length(); i++){
        bool repeated = false;
        for(int j = 0; j < schoolList.length(); j++){
            if ((*list)[i].school == schoolList[j]){
                repeated = true;
            }
        }
        if (!repeated && (*list)[i].school != ""){
            schoolList.append((*list)[i].school);
        }
    }
    int schoolCount[schoolList.length()];
    for(int i = 0; i < sizeof(schoolCount)/sizeof(schoolCount[0]); i++){
        schoolCount[i] = 0;
    }
    for(int i = 0; i < schoolList.length(); i++){
        for(int j = 0; j < list->length(); j++){
            if((*list)[j].school == schoolList[i] && (*list)[j].group == -1){
                schoolCount[i] ++;
            }
        }
    }
    for(int i = 0; i < sizeof(schoolCount)/sizeof(schoolCount[0]); i++){
        if(schoolCount[0] < schoolCount[i]){
            return false;
        }
    }
    return true;
}

void knockOutSys::getTableAndDraw(QTableWidget *table){
    QVector<participants> list;
    participants part;
    for (int i = 0; i < table->rowCount(); i++){
        part.id = table->item(i, 0)->text().toInt();
        part.name = table->item(i, 1)->text();
        part.school = table->item(i, 2)->text();
        if (table->item(i ,3)->text() == "true"){
            part.isSeed = true;
        }
        else{
            part.isSeed = false;
        }
        list.append(part);
    }

    if (log(list.length())/log(2) - floor(log(list.length())/log(2)) != 0){ //Adding a placeholder
        part.isSeed = false;
        part.name = "bye";
        part.school = "";
        int placeHolderToBeAdded = getClosestPowerTo2(list.length()) - list.size();
        for (int i = 0; i < placeHolderToBeAdded; i++){
            part.id = part.id + 1;
            list.append(part);
        }
    }

    int seedNo = seedRemaining(list);
    QVector<int> seedAllocation(static_cast<int>(floor(list.size()/2)), 0);
    QVector<int> seedPlayerIdx;

    //find seed players' id
    for (int i = 0; i < list.size(); i++){
        if (list[i].isSeed){
            seedPlayerIdx.push_back(list[i].id);
        }
    }

    //Calculate each group's no. of seed player(s)
    for (int i = 0; i < static_cast<int>(floor(list.size()/2)); i++){
        if (seedNo == 0){
            break;
        }
        seedAllocation[i] = round(seedNo/(list.size()/2.0 - i));
        seedNo -= seedAllocation[i];
    }

    for (int i = 0; i < list.size()/2; i++){
        participants *fstId = nullptr;
        for (int j = 0; j < seedAllocation[i]; j ++){
            for (int k = 0; k < static_cast<int>(seedPlayerIdx.size()); k++){
                if (fstId == nullptr){
                    if (list[seedPlayerIdx[k]].group == -1){
                        list[seedPlayerIdx[k]].group = i;
                        fstId = &list[seedPlayerIdx[k]];
                        break;
                    }
                }
                else{
                    if (fstId->school != list[seedPlayerIdx[k]].school && list[seedPlayerIdx[k]].group == -1){
                        list[seedPlayerIdx[k]].group = i;
                        break;
                    }
                    if (k == static_cast<int>(seedPlayerIdx.size())-1){
                        for (int l = 0; l < static_cast<int>(seedPlayerIdx.size()); l++){
                            if (list[seedPlayerIdx[l]].group == -1){
                                list[seedPlayerIdx[l]].group = i;
                                break;
                            }
                        }
                    }
                }
            }
        }
    }

    QVector<int> byeIdx;
    for (int i = 0; i < list.size(); i++){
        if (list[i].name == "bye" && list[i].school == ""){
            byeIdx.append(i);
        }
    }
    int byePointer = 0;
    for (int i = 0; i < list.size()/2; i++){
        if (byePointer > byeIdx.size()-1){
            break;
        }
        if(i%(((list.size()/2))/getClosestPowerTo2(byeIdx.size())) == 0){
            list[byeIdx[byePointer]].group = i;
            byePointer++;
        }
        if (byePointer > byeIdx.size()-2){
            break;
        }
        if((list.size()/2 - 1 - i)%(((list.size()/2))/getClosestPowerTo2(byeIdx.size())) == 0){
            list[byeIdx[byePointer]].group = list.size()/2 - 1 - i;
            byePointer++;
        }
    }

    for (int i = 0; i < list.size()/2; i++){
        participants *fst = nullptr, *last = nullptr;
        //try to find first participant in same group
        for (int j = 0; j < list.size(); j++){
            if (list[j].group == i){
                fst = &list[j];
            }
        }
        if (fst != nullptr){//try to find last
            for (int j = 0; j < list.size(); j++){
                if (fst->id != list[j].id && list[j].group == i){
                    last = &list[j];
                    break;
                }
            }
        }
        if (fst == nullptr){
            for (int j = 0; j < list.size(); j++){//find fst
                if (list[j].group == -1 && isFromLargestSchoolGroup(&list, list[j].school)){
                    list[j].group = i;
                    fst = &list[j];
                    break;
                }
            }
        }
        if (last == nullptr && fst != nullptr){
            for (int k = 0; k < list.size(); k++){//find bye
                if (list[k].name == "bye" && list[k].school == "" && list[k].group == -1){
                    last = &list[k];
                    list[k].group = i;
                    break;
                }
            }
            for (int k = 0; k < 2 && last == nullptr; k++){//find last
                for (int j = 0; j < list.size(); j++){
                    if (fst->id != list[j].id && fst->school != list[j].school && k == 0 && list[j].group == -1 && isFromLargestSchoolGroup(&list, list[j].school)){
                        list[j].group = i;
                        last = &list[j];
                        break;
                    }
                    if (last == nullptr && k == 1 && fst->id != list[j].id && list[j].group == -1 && isFromLargestSchoolGroup(&list, list[j].school)){
                        list[j].group = i;
                        last = &list[j];
                        break;
                    }
                }
            }
        }
    }

    scene->clear();
    QVector<double> pts;
    QVector<double> newPts;
    int maxl = maxLen(list);
    srand(time(0));

    for (int i = 0; i < static_cast<int>(floor(list.size()/2)); i++){

        QGraphicsTextItem * fst = new QGraphicsTextItem;
        QGraphicsTextItem * lst = new QGraphicsTextItem;

        QString isSeedStr = "";

        int fstid;
        for (int j = 0; j < list.size(); j++){
            if (list[j].isSeed){
                isSeedStr = "*";
            }
            else{
                isSeedStr = "";
            }
            if (list[j].group == i){
                fstid = list[j].id;
                fst->setPlainText("name:"+list[j].name+"\n"+"school:"+list[j].school+"\n"+"ID"+QString::number(list[j].id)+isSeedStr);
            }
        }
        for (int j = 0; j < list.size(); j++){
            if (list[j].isSeed){
                isSeedStr = "*";
            }
            else{
                isSeedStr = "";
            }
            if (list[j].id != fstid && list[j].group == i){
                lst->setPlainText("name:"+list[j].name+"\n"+"school:"+list[j].school+"\n"+"ID"+QString::number(list[j].id)+isSeedStr);
            }
        }

        if(rand()%2){
            QGraphicsTextItem *temp = fst;
            fst = lst;
            lst = temp;
        }

        fst->setPos(-maxl*8, 120*i);
        scene->addLine(QLineF(0,120*i+25,40,120*i+25), QPen("white"));
        lst->setPos(-maxl*8, 120*i + 65);
        scene->addLine(QLineF(0,120*i+100,40,120*i+100), QPen("white"));

        scene->addLine(QLineF(40,120*i+25,40,120*i+100), QPen("white"));
        pts.push_back((120*i+25+120*i+100)/2);

        scene->addItem(fst);
        scene->addItem(lst);
    }

    for (int i = 0; i < log(list.size())/log(2)-1; i++){
        for (int j = 0; j < pts.size(); j++){
            scene->addLine(QLineF(40*(i+1),pts[j],40*(i+1)+40,pts[j]), QPen("white"));
        }
        for (int j = 0; j < pts.size(); j++){
            if ((j+1)%2 == 0){
                scene->addLine(QLineF(40*(i+1)+40,pts[j-1],40*(i+1)+40,pts[j]), QPen("white"));
                newPts.push_back((pts[j-1]+pts[j])/2);
            }
        }
        pts = newPts;
        newPts.clear();
    }
    scene->addLine(QLineF(40*(log(list.size())/log(2)-1)+40,pts[0],40*(log(list.size())/log(2))+40,pts[0]), QPen("white"));
}
