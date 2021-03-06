#include "cudatconsole.h"

CuDatConsole::CuDatConsole() {
  m_delimeter_str = ";";
  prepareQuery("");
  loadDatabase("");
}

CuDatConsole::CuDatConsole(QString f_dataBasePath, QString f_delimeter_str,
                           QString f_SelectRule_str) {
  m_delimeter_str = f_delimeter_str;
  prepareQuery(f_SelectRule_str);
  loadDatabase(f_dataBasePath);
}

CuDatConsole::~CuDatConsole() {}

void CuDatConsole::showDataList() {}

void CuDatConsole::characterAlignment(QString &f_targetString) {
  f_targetString.remove("\"");

  f_targetString.replace(" =", "=");
  f_targetString.replace("= ", "=");

  f_targetString.replace(" ,", ",");
  f_targetString.replace(", ", ",");
}

void CuDatConsole::prepareQuery(QString f_SelectRule_str) {
  const unsigned int lc_beforeWherePart = 0;
  const unsigned int lc_afterWherePart = 1;
  const unsigned int lc_TargetPart = 1;

  // SELECT name,brand where date="2010" & sex="men" | brand="ktm"
  QString l_tmpSelectRule_str =
      f_SelectRule_str; // on GUI: =ui->querytEdit->toPlainText();
  QString l_queryCommand;

  QStringList l_ruleWherePartDivide;
  QStringList l_selectRuleParts;
  QStringList l_selectTargetsParts;
  QStringList l_selectTargets;

  characterAlignment(l_tmpSelectRule_str);

  // Part0: SELECT name,brand where
  // Part1:date="2010" & sex="men" | brand="ktm"
  l_ruleWherePartDivide =
      l_tmpSelectRule_str.split("where", QString::SkipEmptyParts);

  //(Part0:date="2010") (Part1:&) (Part2:sex="men") (Part3:|)
  //(Part4:brand="ktm")
  l_selectRuleParts = l_ruleWherePartDivide.at(lc_afterWherePart)
                          .split(" ", QString::SkipEmptyParts);

  //(Part0:SELECT) (Part1:name,brand)
  l_selectTargetsParts = l_ruleWherePartDivide.at(lc_beforeWherePart)
                             .split(" ", QString::SkipEmptyParts);
  //(Part0:name) (Part1:brand)
  l_selectTargets = l_selectTargetsParts.at(lc_TargetPart)
                        .split(",", QString::SkipEmptyParts);

  m_selectTargets_v = l_selectTargets.toVector();
  m_selectRule_v = l_selectRuleParts.toVector();
}

void CuDatConsole::vectorConvert(const QVector<QString> &f_selectRule_v,
                                 const QVector<QString> &f_selectTargets_v) {
  for (QString l_ruleMember_str : f_selectRule_v) {
    m_selectRule_stdv.push_back(l_ruleMember_str.toStdString());
  }

  for (QString l_targetMember_str : f_selectTargets_v) {
    m_selectTargets_stdv.push_back(l_targetMember_str.toStdString());
  }
}

void CuDatConsole::loadDatabase(QString f_dataBasePath) {

  CSVReader *reader = new CSVReader(f_dataBasePath, m_delimeter_str);
  m_dataList_v = reader->getData();

  m_SelectProcess.loadDatabase(m_dataList_v);
  // showDataList();

  delete reader;
}

void CuDatConsole::runQuery() {

  vectorConvert(m_selectRule_v, m_selectTargets_v);

  m_SelectProcess.readSelectRule(m_selectRule_stdv);
  m_SelectProcess.run();
  m_SelectProcess.showDatabase();

  m_selectRule_stdv.clear();
}
