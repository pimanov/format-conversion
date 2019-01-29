/**@mainpage 
 * Данная программа - решает первое задание от Яандекса для стежеров.
 * Программа считывает из потока ввода структуру некоторой директории с поддиректориями 
 * в одном формате и выдает ее на печать в другом. 
 * Формат один из шести: find, python, acm1, acm2, acm3, xml.  
 * 
 * В программе реализован класс Converter, его конструктор считывает из потока 
 * ввода структуру каталога в формате, переданном в параметрах конструктору и 
 * переводит данные во внутренние представление. По вызову специальной функции 
 * данные выводятся на печать в другом формате, переданном в параметрах.
 * 
 * Если входной и выходной форматы одинаковы, такой подход не эффективен. 
 * Достаточно лишь посимвольно скопировать входной поток данных в выходной. 
 * Если, например, информация вводится в формате python, а вывести ее нужно в 
 * формате xml, реализованный подход также не эффективен, можно построчно 
 * считавать ввод, добавлять теги и выводить результат на печать.
 * Но, если нужно ограничить максимальное время работы программы, то есть 
 * ограничить время конвертации для каждой пары форматов, данный подход 
 * представляется целесобразным. 
 * 
 * Для хранения сьруктуры каталогов кажется разумным использовать древовидную 
 * структуру, но в данной задаче это не рационально, так как в форматах acm, 
 * например, необходимо выводить все названия каталогов, упорядочекнные по ID.
 * Поэтомы в данной программе каталог представляется структурой, в которой 
 * хранятся ссылки на дочерние и родительские каталоги и имая каталога. 
 * Все каталоги хранятся в map с собственным ID в качестве ключа.  
 * 
 * @author Пиманов Владимир (Pimanov.Vladimir@gmail.com)
 */
#include <string>
#include <iostream>
#include <map>
#include <vector>
#include <sstream>
#include <algorithm>
using namespace std;

/**@class Converter
 * Хранит представление структуры каталогов и функции для их считывания и печати
 * в различных форматах. 
 */
class Converter
{
    /**@note в данной програме понятия файл и директория отождествлены. Все называется Dir.
     * @note суффикс "_r" в названии функции - рекурсивная функция
     */
    
    ///@name Внутренние типы
    ///@{
    /// Хранит число директорий (от 0 до 2^31-1), либо число отступов при печати 
    /// в таких форматах, как python 
    typedef long DirsNumber; 
    
    ///Тип для ID директорий
    typedef long ID; 
    
    /// тип для массива из ID
    typedef vector<ID> IDs;
    
    /// структура для представления директории
    typedef struct{
       string name;
       ID parentID;
       IDs childrenIDs;
    } Dir;
    
    /// Тип для множества все директорий
    typedef map<ID,Dir> Dirs; // <ID,Dir> - key is the ID of this Dir
    
    /// вспомогательный тип для вывода директорий в формате find
    typedef map<ID,IDs> PathMap; // to printFind(); IDs = vector<ID> contains absolute path to the dir, key - dir ID 
    
    /// константа, неопределнный ID
    static const ID nullID = -1;
    ///@}
    
    ///@name переменные 
    ///@{
    /// ID корневого каталога
    ID rootID;
    
    ///Все директории
    Dirs dirs;  // contain all dirs
    ///@}

    
    /// @name Вспомогательные функции
    /// @{
    void changeID(ID oldID, ID newID); // ID of rootDir can't be changed by this function
    void pushDir(ID parentID, ID dirID, string name); // rootDir can't be pushed by this function
    void scanAcmBase(); // AcmBase - common part in all acm format
    void printPython_r(ID dirID, DirsNumber tabsNumber); // _r - recursion fo printPython()
    void printXml_r(ID dirID, DirsNumber tabsNumber); // _r - recursion for printXml()
    void printAcmBase(); 
    void initPathMap(PathMap &pathMap);
    void initPathMap_r (ID dirID, PathMap &pathMap); // _r - recursion for initPathMap()
    void scanXml_r(ID parentID); // _r - recursion for scanXml()
    void scanPython_r (DirsNumber &linesNumber, string &line, DirsNumber tabsNumber, ID parentID); // _r - recursion fo scanPython()
    static void printFileXmlTag( const string name, const ID id, const DirsNumber tabsNumber );
    static void printOpenDirXmlTag( const string name, const ID id, const DirsNumber tabsNumber );
    static void printCloseDirXmlTag( const DirsNumber tabsNumber );
    ///@}
    
    
    ///@name Функции, читающие поток ввода и заполняющие внутреннее представление
    ///@{
    void scanFind();
    void scanPython();
    void scanAcm1();
    void scanAcm2();
    void scanAcm3();
    void scanXml();
    ///@}
    
    ///@name Функции печать 
    ///@{
    void printFind();
    void printPython();
    void printAcm1();
    void printAcm2();
    void printAcm3();
    void printXml();
    ///@}
    
public:
    ///@name конструктор/диструктор
    ///@{
    Converter (string format);
    void print (string format);
    ///@}
};


Converter::Converter (string format)
{
    if (format == "find") scanFind();
    if (format == "python") scanPython();
    if (format == "acm1") scanAcm1();
    if (format == "acm2") scanAcm2();
    if (format == "acm3") scanAcm3();
    if (format == "xml") scanXml();
}
    
void Converter::print(string format)
{
    if (format == "find") printFind();
    if (format == "python") printPython();
    if (format == "acm1") printAcm1();
    if (format == "acm2") printAcm2();
    if (format == "acm3") printAcm3();
    if (format == "xml") printXml();
}


void Converter::printXml_r(ID dirID, DirsNumber tabsNumber)
{
    Dir *dir = &(dirs[dirID]);
    IDs *childrenIDs = &(dir->childrenIDs);
        
    if(childrenIDs->empty()) printFileXmlTag(dir->name, dirID, tabsNumber);
    else {
        printOpenDirXmlTag(dir->name, dirID, tabsNumber);
        
        for(IDs::const_iterator childID = childrenIDs->begin(); childID != childrenIDs->end(); childID++)
            printXml_r (*childID, tabsNumber + 1 );
        
        printCloseDirXmlTag(tabsNumber);
    }
}

void Converter::printXml()
{
    printXml_r(rootID, 0);
}

void Converter::printPython_r(ID dirID, DirsNumber tabsNumber)
{
    Dir *dir = &(dirs[dirID]);
    
    for(int i=0; i<tabsNumber; i++) cout << "    ";
    cout << dir->name << " " << dirID << endl;
    
    IDs *childrenIDs = &(dir->childrenIDs);
    for(IDs::const_iterator childID = childrenIDs->begin(); childID != childrenIDs->end(); childID++)
        printPython_r (*childID, tabsNumber + 1 );
}

void Converter::printPython()
{
    cout << dirs.size() << endl;
    
    printPython_r(rootID, 0);
}


void Converter::printAcmBase()
{
    cout << dirs.size() << endl;
    
    for(Dirs::const_iterator dir = dirs.begin(); dir != dirs.end(); dir++)
        cout << dir->second.name << " " << dir->first << endl;
}

void Converter::printAcm1()
{
    printAcmBase();

    for(Dirs::const_iterator dir = dirs.begin(); dir != dirs.end(); dir++)
    {
        const IDs *childrenIDs = &(dir->second.childrenIDs);
        
        cout << childrenIDs->size();
        
        for(IDs::const_iterator childID = childrenIDs->begin(); childID != childrenIDs->end(); childID++)
            cout << " " << *childID;
        
        cout << endl;
    }
}

void Converter::printAcm2()
{
    printAcmBase();
    
    for(Dirs::const_iterator dir = dirs.begin(); dir != dirs.end(); dir++)
        if(dir->first != rootID) cout << dir->second.parentID << endl;
        else cout << "-1" << endl;
}

void Converter::printAcm3()
{
    printAcmBase();
    
    for(Dirs::const_iterator dir = dirs.begin(); dir != dirs.end(); dir++)
    {
        const ID dirID = dir->first;
        const IDs *childrenIDs = &(dir->second.childrenIDs);
         
        for(IDs::const_iterator childID = childrenIDs->begin(); childID != childrenIDs->end(); childID++)
             cout << dirID << " " << *childID << endl; 
    }
}


void Converter::printFind()
{
    cout << dirs.size() << endl;
    
    PathMap pathMap;
    initPathMap(pathMap); // pathMap contain all absolute path ID without rootDirID with DirID

    string rootName = dirs[rootID].name;
    
    for(PathMap::const_iterator dir = pathMap.begin(); dir != pathMap.end(); dir++)
    {
        const ID dirID = dir->first;
        const IDs *dirPath = &(dir->second);
        
        cout << rootName;
        for(IDs::const_iterator id  = dirPath->begin(); id != dirPath->end(); id++)
            cout << "/" << dirs[*id].name;
        
        cout << " " << dirID << endl;
    }
}


void Converter::scanXml_r (ID parentID)
{
    string buf;
    cin >> buf;
    
    if(buf == "<dir")
    {
        Dir dir;
        ID dirID;
        
        getline(cin,buf,'\'');
        getline(cin,dir.name,'\'');
        
        getline(cin,buf,'\'');
        cin >> dirID;
        getline(cin,buf);
        
        dir.parentID = parentID;
        dirs[dirID] = dir;
        
        scanXml_r(dirID);
        
        if(parentID != nullID) scanXml_r(parentID);
    }
    else if(buf == "<file")
    {
        Dir dir;
        ID dirID;
        
        getline(cin,buf,'\'');
        getline(cin,dir.name,'\'');
        
        getline(cin,buf,'\'');
        cin >> dirID;
        getline(cin,buf);
        
        dir.parentID = parentID;
        dirs[dirID] = dir;
        
        scanXml_r(parentID);
    }
    else if(buf == "</dir>")
    {
    }
}

void Converter::scanXml()
{
    scanXml_r(nullID);
    
    // fill Child and rootID
    for(Dirs::const_iterator dir = dirs.begin(); dir != dirs.end(); dir++)
    {
        const ID *dirID = &(dir->first); 
        const ID *parentID = &(dir->second.parentID);
        
        if(*parentID == nullID) rootID = *dirID;
        else dirs[*parentID].childrenIDs.push_back(*dirID);
    }
}


void Converter::scanAcmBase()
{
    DirsNumber dirsNumber;
    cin >> dirsNumber;
 
    for(DirsNumber i=0; i<dirsNumber; i++)
    {
        Dir dir;
        ID dirID;
        cin >> dir.name >> dirID;
        dir.parentID = nullID;
        
        dirs[dirID] = dir;
    }
}

void Converter::scanAcm3()
{
    scanAcmBase();
    
    for(int i = 1; i < dirs.size(); i++)
    {
        ID parentID, childID;
        cin >> parentID >> childID;
        
        dirs[parentID].childrenIDs.push_back(childID);
        dirs[childID].parentID = parentID;
    }
    
    //find root dir, rootDir.parentID = nullID
    rootID = nullID;
    for(Dirs::const_iterator dir = dirs.begin(); rootID == nullID; dir++)
        if(dir->second.parentID == nullID) rootID = dir->first;
}

void Converter::scanAcm2()
{
    scanAcmBase();
    
    for(Dirs::iterator dir = dirs.begin(); dir != dirs.end(); dir++)
    {
        const ID dirID = dir->first;
        ID parentID;
        cin >> parentID;
        dir->second.parentID = parentID;
        if (parentID == nullID) rootID = dirID;
        else dirs[parentID].childrenIDs.push_back(dirID);
    }
}

void Converter::scanAcm1()
{
    scanAcmBase();
    
    //scan second part
    for(Dirs::iterator dir = dirs.begin(); dir != dirs.end(); dir++)
    {
        const ID dirID = dir->first;
        
        DirsNumber childrenNumber;
        cin >> childrenNumber;
        
        for(DirsNumber j=0; j<childrenNumber; j++)
        {
            ID childID;
            cin >> childID;
            dir->second.childrenIDs.push_back(childID);
            dirs[childID].parentID = dirID;
        }
    }
    
    // find root dir, rootDir.parentID = nullID
    rootID = nullID;
    for(Dirs::const_iterator dir = dirs.begin(); rootID == nullID; dir++)
        if(dir->second.parentID == nullID) rootID = dir->first;
}



void Converter::scanPython_r (DirsNumber &linesNumber, string &line, DirsNumber tabsNumber, ID parentID)
{
    int j = 0;
    while(line[j] == ' ') j++;
    DirsNumber realTabsNumber = j / 4;

    if(realTabsNumber == tabsNumber)
    {
        Dir dir;
        ID dirID;

        istringstream isline(line);
        isline >> dir.name >> dirID;

        dir.parentID = parentID;
        dirs[dirID] = dir;
        
        if(linesNumber > 0)
        {
            linesNumber--;
            getline(cin,line);

            scanPython_r(linesNumber, line, tabsNumber+1, dirID);
            scanPython_r(linesNumber, line, tabsNumber, parentID);
        }
    }
}

void Converter::scanPython()
{
    DirsNumber linesNumber;
    cin >> linesNumber;
    
    string line;
    getline(cin,line); // read final enter, line = ""
    
    linesNumber--;
    getline(cin,line);
    
    scanPython_r(linesNumber, line, 0, nullID); // line - last readed line
    
    //fill Child and rootID
    for(Dirs::const_iterator dir = dirs.begin(); dir != dirs.end(); dir++)
    {
        const ID *dirID = &(dir->first); 
        const ID *parentID = &(dir->second.parentID);
        
        if(*parentID == nullID) rootID = *dirID;
        else dirs[*parentID].childrenIDs.push_back(*dirID);
    }
}



void Converter::scanFind()
{
    // при чтении cin, как только встречается новое имя, дириктория с этим именем добавляется в dirs с временным id.
    // как только id становится известен, временный id заменяется на натоящий функцией changeID().
    // случаи работы с root обрабатываются отдельно.  
    
    DirsNumber dirsNumber;
    cin >> dirsNumber;
 
    ID tempID = -1; // tempID in [-2^31, -1], tempID--
    rootID = tempID--;
    
    for(int i=0; i<dirsNumber; i++)
    {
        string absolutePath;
        ID dirID;
        cin >> absolutePath >> dirID;
        
        if(i==0){ // first time, push root dir
            Dir dir;
            dir.name = absolutePath.substr(0,absolutePath.find('/'));
            dir.parentID = nullID;
            dirs[rootID] = dir;
        }
        

        
        if(absolutePath.find('/') == string::npos) // root ID is in this line
        {
            dirs[dirID] = dirs[rootID];
            dirs.erase(rootID);
            rootID = dirID;
            
            Dir *dir = &(dirs[rootID]);
            IDs *childrenIDs = &(dir->childrenIDs);
            for(IDs::iterator childID = childrenIDs->begin(); childID != childrenIDs->end(); childID++)
                dirs[*childID].parentID = dirID;
        }
        else
        {
            ID parentID = rootID; 
            size_t lS = absolutePath.find('/') + 1;
            size_t nS = absolutePath.find('/', lS);
            for( ;nS != string::npos; lS = nS+1, nS = absolutePath.find('/',nS+1))
            {
                //unknown real dirID
                string dirName = absolutePath.substr(lS, nS-lS);

                IDs *parChilds = &(dirs[parentID].childrenIDs);
                for(IDs::iterator id = parChilds->begin();; id++)
                {
                    if (id == parChilds->end()) {
                        //dirName has met first time
                        ID dirID = tempID--;
                        pushDir(parentID, dirID, dirName);
                        parentID = dirID;
                        break;
                    }
                    else if (dirs[*id].name == dirName) {
                        //dir with this name is in dirs
                        parentID = *id;
                        break;
                    }
                }
            }

            // real dirID are known
            string dirName = absolutePath.substr(lS);

            IDs *parChilds = &(dirs[parentID].childrenIDs);
            for(IDs::iterator id = parChilds->begin();; id++)
            {
                if( id == parChilds->end()) {
                    //dirName has met first time
                    pushDir(parentID, dirID, dirName);
                    break;
                }
                else if(dirs[*id].name == dirName){
                    //dir with this name is in dirs
                    changeID(*id,dirID);
                    break;
                }
            }
        }
    }
    
    for(Dirs::iterator dir = dirs.begin(); dir != dirs.end(); dir++)
        sort(dir->second.childrenIDs.begin(), dir->second.childrenIDs.end());
}

void Converter::initPathMap_r (ID dirID, PathMap &pathMap)
{
    const Dir *dir = &(dirs[dirID]);
    const IDs *childrenIDs = &(dir->childrenIDs);
    IDs *dirPath = &(pathMap[dirID]);
    dirPath->push_back(dirID);
    
    for(IDs::const_iterator childID = childrenIDs->begin(); childID != childrenIDs->end(); childID++)
    {
        pathMap[*childID] = *dirPath;       
        initPathMap_r(*childID, pathMap);
    }
}

void Converter::initPathMap(PathMap &pathMap)
{
    const Dir *rootDir = &(dirs[rootID]);
    const IDs *rootChildren = &(rootDir->childrenIDs);
    
    pathMap[rootID] = IDs();
    for(IDs::const_iterator rootChild = rootChildren->begin(); rootChild != rootChildren->end(); rootChild++)
    {
        pathMap[*rootChild] = IDs();
        initPathMap_r (*rootChild, pathMap);
    }
}


void Converter::printFileXmlTag (const string name, const ID id, const DirsNumber tabsNumber)
{
    for(int i=0; i< tabsNumber; i++ ) cout << "  ";
    cout << "<file name='" << name << "' id='" << id << "'/>" << endl;
}

void Converter::printOpenDirXmlTag (const string name, const ID id, const DirsNumber tabsNumber)
{
    for(int i=0; i< tabsNumber; i++ ) cout << "  ";
    cout << "<dir name='" << name << "' id='" << id << "'>" << endl;
}

void Converter::printCloseDirXmlTag (const DirsNumber tabsNumber)
{
    for(int i=0; i< tabsNumber; i++ ) cout << "  ";
    cout << "</dir>" << endl;
}


void Converter::changeID(ID oldID, ID newID)
{
    Dir dir = dirs[oldID];
    
    IDs *childrenIDs = &(dirs[dir.parentID].childrenIDs);
    for(IDs::iterator childID = childrenIDs->begin(); ; childID++)
        if(*childID == oldID){
            *childID = newID;
            break;
        }
    
    childrenIDs = &(dir.childrenIDs);
    for(IDs::iterator childID = childrenIDs->begin(); childID != childrenIDs->end(); childID++)
        dirs[*childID].parentID = newID;
    
    dirs[newID] = dir;
    dirs.erase(oldID);
}

void Converter::pushDir(ID parentID, ID dirID, string name)
{
    dirs[parentID].childrenIDs.push_back(dirID);
    Dir dir;
    dir.name = name;
    dir.parentID = parentID;
    dirs[dirID] = dir;
}




int main()
{
    string inputFormat, outputFormat;
    cin >> inputFormat >> outputFormat;
    
    Converter convertor(inputFormat);
    convertor.print(outputFormat);
    
    return 0;
}