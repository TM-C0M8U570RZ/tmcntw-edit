#include "tmc_application.h"
#include "tmc_global_prompts.h"
#include <tmcndbtagging.h>
#include <sstream>

void printHelp();

void funE();

void removeChildFrom(std::string tagStr, std::shared_ptr<tmc::Bidoof::NdbTag>& nt);

void removeParentFrom(std::string tagStr, std::shared_ptr<tmc::Bidoof::NdbTag>& nt);

void deleteTag(std::shared_ptr<tmc::Bidoof::NdbTag>& nt, tmc::Bidoof::NdbTagWeb& ntw);

void retypeTo(std::string arg, std::shared_ptr<tmc::Bidoof::NdbTag>& nt, tmc::Bidoof::NdbTagWeb& ntw);

void addParentTo(std::string arg, std::shared_ptr<tmc::Bidoof::NdbTag>& nt, tmc::Bidoof::NdbTagWeb& ntw);

void addChildTo(std::string arg, std::shared_ptr<tmc::Bidoof::NdbTag>& nt, tmc::Bidoof::NdbTagWeb& ntw);

void orphan(std::shared_ptr<tmc::Bidoof::NdbTag>& nt);

void endBloodline(std::shared_ptr<tmc::Bidoof::NdbTag>& nt);

void addAliasTo(std::string arg, std::shared_ptr<tmc::Bidoof::NdbTag>& nt);

void removeAliasFrom(std::string arg, std::shared_ptr<tmc::Bidoof::NdbTag>& nt);

void clearAliases(std::shared_ptr<tmc::Bidoof::NdbTag>& nt);

void setNndetectEnable(std::shared_ptr<tmc::Bidoof::NdbTag>& nt, bool enabled);

void setAgeGroup(std::shared_ptr<tmc::Bidoof::NdbTag>& nt, std::string ageGroup);

void queryInfo(std::shared_ptr<tmc::Bidoof::NdbTag>& nt, tmc::Bidoof::NdbTagWeb& ntw);

void printTree(tmc::Bidoof::NdbTagWeb& ntw);

void mergeFiles(tmc::Bidoof::NdbTagWeb& in, tmc::Bidoof::NdbTagWeb& out,
                std::vector<std::string> filter = std::vector<std::string>(),
                std::shared_ptr<tmc::Bidoof::NdbTag> dest = nullptr, bool prompt = true, bool overwrite = false);

void recursiveTagPrint(std::shared_ptr<tmc::Bidoof::NdbTag>& nt, u64 depth = 0);

int main(int argc, char* argv[])
{
    tmc::Application a(argc, argv);
    if (argc == 1)
    {
        std::string path;
        std::shared_ptr<tmc::Bidoof::NdbTag> nt;
        u32 choice = 0;
        while (path.empty() || std::filesystem::is_directory(path))
        {
            std::cout << "Please type (or paste) path to .tmcntw and press enter: ";
            std::getline(std::cin, path, '\n');
        }
        if (!std::filesystem::exists(path))
        {
            std::cout << "\nFile: \"" << path << "\" Doesn't exist.\n"
                      << "It will be created.\n";
        }
        tmc::Bidoof::NdbTagWeb ntw(path);
        while (choice != 5)
        {
            choice = tmc::namedPrompt("\nFile Menu:\n", "\nYour choice: ", {"a", "r", "t", "p",
                                                                          "w", "q"},
                                      {"Add a new tag", "Remove a tag", "Enter tag submenu",
                                       "Print tree", "Write file", "Quit program"});
            if (choice == 0)
            {
                std::cout << "\nEnter tag (no spaces plez): ";
                std::string tagStr;
                std::cin >> tagStr;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                tagStr = tmc::toAllLower(tagStr);
                u8 nndetect = tmc::yesNoPrompt("\nShould this tag indicate libTmcNnDetect protection? ");
                u8 ageRating = static_cast<u8>(tmc::namedPrompt("\nChoose content rating\n",
                                                                "\nYour choice: ",
                                                                {"s", "q", "e"},
                                                                {"Safe", "Questionable", "Explicit"}));
                if (ageRating == 2) ageRating++;
                tmc::Bidoof::NdbTag::AxceModYum am = static_cast<tmc::Bidoof::NdbTag::AxceModYum>(
                    (nndetect << 7) | ageRating);
                tmc::Bidoof::NdbTag::Category cat = static_cast<tmc::Bidoof::NdbTag::Category>(
                    tmc::namedPrompt("\nChoose tag category\n", "\nYour choice: ",
                                     {"r", "a", "c", "s", "g", "m", "l"},
                                     {"Copyright", "Author", "Character",
                                      "Species", "General", "Meta", "Lore"}));
                std::cout << "\nPlease enter space-separated aliases.\n"
                          << "Just hit enter for no aliases: ";
                std::string aliasGlob;
                std::getline(std::cin, aliasGlob, '\n');
                aliasGlob = tmc::toAllLower(aliasGlob);
                std::stringstream sstr(aliasGlob);
                std::vector<std::string> aliases;
                while (!sstr.eof())
                {
                    std::string temp;
                    std::getline(sstr, temp, ' ');
                    aliases.push_back(temp);
                }
                if (cat == tmc::Bidoof::NdbTag::COPYRIGHT)
                {
                    nt = std::make_shared<tmc::Bidoof::CopywrongTag>(
                        am, tagStr, std::vector<std::shared_ptr<tmc::Bidoof::NdbTag>>(0), aliases);
                }
                else if (cat == tmc::Bidoof::NdbTag::AUTHOR)
                {
                    nt = std::make_shared<tmc::Bidoof::AuthorTag>(
                        am, tagStr, std::vector<std::shared_ptr<tmc::Bidoof::NdbTag>>(0), aliases);
                }
                else if (cat == tmc::Bidoof::NdbTag::CHARACTER)
                {
                    nt = std::make_shared<tmc::Bidoof::CharTag>(
                        am, tagStr, std::vector<std::shared_ptr<tmc::Bidoof::NdbTag>>(0), aliases);
                }
                else if (cat == tmc::Bidoof::NdbTag::SPECIES)
                {
                    nt = std::make_shared<tmc::Bidoof::SpeciesTag>(
                        am, tagStr, std::vector<std::shared_ptr<tmc::Bidoof::NdbTag>>(0), aliases);
                }
                else if (cat == tmc::Bidoof::NdbTag::GENERAL)
                {
                    nt = std::make_shared<tmc::Bidoof::GeneralTag>(
                        am, tagStr, std::vector<std::shared_ptr<tmc::Bidoof::NdbTag>>(0), aliases);
                }
                else if (cat == tmc::Bidoof::NdbTag::META)
                {
                    nt = std::make_shared<tmc::Bidoof::MetaTag>(
                        am, tagStr, std::vector<std::shared_ptr<tmc::Bidoof::NdbTag>>(0), aliases);
                }
                else if (cat == tmc::Bidoof::NdbTag::LORE)
                {
                    nt = std::make_shared<tmc::Bidoof::LoreTag>(
                        am, tagStr, std::vector<std::shared_ptr<tmc::Bidoof::NdbTag>>(0), aliases);
                }
                if (!ntw.containsTagHierarchy(nt->getTagString()))
                {
                    ntw.addTopLevelTag(nt);
                }
                else
                {
                    std::cout << "\nTag already exists!\n";
                }
            }
            else if (choice == 1)
            {
                std::cout << "\nWhich tag should be ELIMINATED? ";
                std::string tagStr;
                std::cin >> tagStr;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                nt = ntw[tagStr];
                if (nt)
                {
                    deleteTag(nt, ntw);
                }
                else
                {
                    std::cout << "\nThat tag doesn't exist.\n";
                }
            }
            else if (choice == 2)
            {
                std::cout << "\nEnter a tag to open the menu with: ";
                std::string tagStr;
                std::cin >> tagStr;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                nt = ntw[tagStr];
                if (nt)
                {
                    u32 choice = 0;
                    while (choice != 14)
                    {
                        choice = tmc::indexedPrompt("\nTag Menu\n", "\nYour choice: ",
                                                    {"Set tag name", "Set category",
                                                     "Enable DRM protection",
                                                     "Set content rating", "Add alias",
                                                     "Remove alias", "Clear aliases",
                                                     "Add child", "Remove child",
                                                     "Clear children", "Add parent",
                                                     "Remove parent", "Orphan",
                                                     "Query tag info", "Go to previous menu"});
                        if (choice == 0)
                        {
                            std::cout << "Please enter new tag name: ";
                            std::string tagStr2;
                            std::cin >> tagStr2;
                            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                            nt->setTagString(tmc::toAllLower(tagStr2));
                        }
                        else if (choice == 1)
                        {
                            tmc::Bidoof::NdbTag::Category cat = static_cast<tmc::Bidoof::NdbTag::Category>(
                                tmc::namedPrompt("\nChoose tag category\n", "\nYour choice: ",
                                                 {"r", "a", "c", "s", "g", "m", "l"},
                                                 {"Copyright", "Author", "Character",
                                                  "Species", "General", "Meta", "Lore"}));
                            std::shared_ptr<tmc::Bidoof::NdbTag> nt2;
                            if (cat == tmc::Bidoof::NdbTag::COPYRIGHT)
                            {
                                nt2 = tmc::Bidoof::ndb_tag_cast<tmc::Bidoof::CopywrongTag>(nt);
                            }
                            else if (cat == tmc::Bidoof::NdbTag::AUTHOR)
                            {
                                nt2 = tmc::Bidoof::ndb_tag_cast<tmc::Bidoof::AuthorTag>(nt);
                            }
                            else if (cat == tmc::Bidoof::NdbTag::CHARACTER)
                            {
                                nt2 = tmc::Bidoof::ndb_tag_cast<tmc::Bidoof::CharTag>(nt);
                            }
                            else if (cat == tmc::Bidoof::NdbTag::SPECIES)
                            {
                                nt2 = tmc::Bidoof::ndb_tag_cast<tmc::Bidoof::SpeciesTag>(nt);
                            }
                            else if (cat == tmc::Bidoof::NdbTag::GENERAL)
                            {
                                nt2 = tmc::Bidoof::ndb_tag_cast<tmc::Bidoof::GeneralTag>(nt);
                            }
                            else if (cat == tmc::Bidoof::NdbTag::META)
                            {
                                nt2 = tmc::Bidoof::ndb_tag_cast<tmc::Bidoof::MetaTag>(nt);
                            }
                            else if (cat == tmc::Bidoof::NdbTag::LORE)
                            {
                                nt2 = tmc::Bidoof::ndb_tag_cast<tmc::Bidoof::LoreTag>(nt);
                            }
                            deleteTag(nt, ntw);
                            if (nt2->getParentCount() == 0 && !ntw.containsTopLevelTag(nt2->getTagString()))
                            {
                                ntw.addTopLevelTag(nt2);
                                nt = nt2;
                            }
                        }
                        else if (choice == 2)
                        {
                            u8 nndetect = tmc::yesNoPrompt("\nShould this tag indicate libTmcNnDetect protection? ");
                            tmc::Bidoof::NdbTag::AxceModYum am = nt->getAccessModifier();
                            nt->setAccessModifier(static_cast<tmc::Bidoof::NdbTag::AxceModYum>(
                                (nndetect << 7) | (am & 3)));
                        }
                        else if (choice == 3)
                        {
                            u8 ageRating = static_cast<u8>(tmc::namedPrompt("\nChoose content rating\n",
                                                                            "\nYour choice: ",
                                                                            {"s", "q", "e"},
                                                                            {"Safe", "Questionable", "Explicit"}));
                            if (ageRating == 2) ageRating++;
                            tmc::Bidoof::NdbTag::AxceModYum am = nt->getAccessModifier();
                            nt->setAccessModifier(static_cast<tmc::Bidoof::NdbTag::AxceModYum>(
                                (am & 0x80) | (ageRating & 3)));
                        }
                        else if (choice == 4)
                        {
                            std::cout << "\nPlease enter alias to add: ";
                            std::string alias;
                            std::cin >> alias;
                            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                            alias = tmc::toAllLower(alias);
                            if (!nt->containsAlias(alias)) nt->addAlias(alias);
                            else std::cout << "\nAlias already exists!\n";
                        }
                        else if (choice == 5)
                        {
                            std::cout << "\nPlease enter alias to delete: ";
                            std::string alias;
                            std::cin >> alias;
                            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                            alias = tmc::toAllLower(alias);
                            if (nt->containsAlias(alias)) nt->removeAlias(alias);
                            else std::cout << "\nAlias doesn't exist!\n";
                        }
                        else if (choice == 6)
                        {
                            nt->getAliases().clear();
                        }
                        else if (choice == 7)
                        {
                            std::cout << "\nPlease enter tag to add as child: ";
                            std::string tagStr2;
                            std::cin >> tagStr2;
                            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                            std::shared_ptr<tmc::Bidoof::NdbTag> nt2 = ntw[tagStr2];
                            if (nt2 && !nt2->containsParentRecursive(nt->getTagString()))
                            {
                                nt->addChild(nt2);
                            }
                            else if (nt2->containsParentRecursive(nt->getTagString()))
                            {
                                std::cout << "\nThat tag is already a child of this tag.\n";
                            }
                            else
                            {
                                std::cout << "\nThat tag doesn't exist.\n";
                            }
                        }
                        else if (choice == 8)
                        {
                            std::cout << "\nPlease enter child to remove: ";
                            std::string tagStr2;
                            std::cin >> tagStr2;
                            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                            std::shared_ptr<tmc::Bidoof::NdbTag> nt2 = ntw[tagStr2];
                            if (nt2 && nt2->containsParentRecursive(nt->getTagString()))
                            {
                                removeParentFrom(nt->getTagString(), nt2);
                            }
                            else if (!nt2->containsParentRecursive(nt->getTagString()))
                            {
                                std::cout << "\nThat tag isn't a child of this tag.\n";
                            }
                            else
                            {
                                std::cout << "\nThat tag doesn't exist.\n";
                            }
                        }
                        else if (choice == 9)
                        {
                            while (nt->getChildCount() != 0)
                            {
                                nt->removeChild(0);
                            }
                        }
                        else if (choice == 10)
                        {
                            std::cout << "\nPlease enter tag to add as parent: ";
                            std::string tagStr2;
                            std::cin >> tagStr2;
                            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                            std::shared_ptr<tmc::Bidoof::NdbTag> nt2 = ntw[tagStr2];
                            if (nt2 && !nt2->containsChildRecursive(nt->getTagString()))
                            {
                                nt->addParent(nt2);
                            }
                            else if (nt2->containsChildRecursive(nt->getTagString()))
                            {
                                std::cout << "\nThat tag is already a parent of this tag.\n";
                            }
                            else
                            {
                                std::cout << "\nThat tag doesn't exist.\n";
                            }
                        }
                        else if (choice == 11)
                        {
                            std::cout << "\nPlease enter parent to remove: ";
                            std::string tagStr2;
                            std::cin >> tagStr2;
                            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                            std::shared_ptr<tmc::Bidoof::NdbTag> nt2 = ntw[tagStr2];
                            if (nt2 && nt2->containsChildRecursive(nt->getTagString()))
                            {
                                removeChildFrom(nt->getTagString(), nt2);
                            }
                            else if (!nt2->containsChildRecursive(nt->getTagString()))
                            {
                                std::cout << "\nThat tag isn't a parent of this tag.\n";
                            }
                            else
                            {
                                std::cout << "\nThat tag doesn't exist.\n";
                            }
                        }
                        else if (choice == 12)
                        {
                            while (nt->getParentCount() != 0)
                            {
                                nt->removeParent(0);
                            }
                        }
                        else if (choice == 13)
                        {
                            queryInfo(nt, ntw);
                        }
                    }
                }
            }
            else if (choice == 3)
            {
                printTree(ntw);
            }
            else if (choice == 4)
            {
                ntw.writeOut();
            }
        }
    }
    else if (!(a.hasArg('t') || a.hasArg('m') || a.hasArg('l')) || a.hasArg('h')
               || a.hasArg("--help"))
    {
        printHelp();
        return -1;
    }
    else if (a.hasArg('m'))
    {
        std::vector<std::string> inpaths = a.getAs<std::vector<std::string>>('m');
        std::vector<tmc::Bidoof::NdbTagWeb> in;
        std::vector<std::string> filter;
        bool overwrite = true;
        bool prompt = true;
        if (a.hasArg('s'))
        {
            filter = a.getAs<std::vector<std::string>>('s');
        }
        std::shared_ptr<tmc::Bidoof::NdbTag> dest = nullptr;
        tmc::Bidoof::NdbTagWeb out = tmc::Bidoof::NdbTagWeb(a.getFilePaths()[0].string());
        if (a.hasArg('d'))
        {
            dest = out[a.getAs<std::string>('d')];
        }
        for (u64 i = 0; i < inpaths.size(); i++)
        {
            in.emplace_back(inpaths[i]);
        }
        if (a.hasArg('n') || a.hasArg("no"))
        {
            overwrite = false;
            prompt = false;
        }
        else if (a.hasArg('y') || a.hasArg("yes"))
        {
            prompt = false;
        }
        for (u64 i = 0; i < in.size(); i++)
        {
            mergeFiles(in[i], out, filter, dest, prompt, overwrite);
        }
        printTree(out);
        out.writeOut();
    }
    else if (a.hasArg('l'))
    {
        tmc::Bidoof::NdbTagWeb ntw(a.getFilePaths()[0]);
        printTree(ntw);
    }
    else
    {
        std::string tagStr = a.getAs<std::string>('t');
        std::shared_ptr<tmc::Bidoof::NdbTag> tag = std::make_shared<tmc::Bidoof::CopywrongTag>(
            tmc::Bidoof::NdbTag::RS, tagStr, std::vector<std::shared_ptr<tmc::Bidoof::NdbTag>>(), std::vector<std::string>());
        std::vector<std::filesystem::path> scrotum = a.getFilePaths();
        std::vector<tmc::Bidoof::NdbTagWeb> balls;
        for (const auto& p: scrotum)
        {
            if (std::filesystem::is_regular_file(p))
            {
                balls.emplace_back(p.string());
            }
            else if (!std::filesystem::exists(p))
            {
                balls.emplace_back();
                balls.back().setFilename(p.string());
            }
        }
        for (u64 i = 0; i < balls.size(); i++)
        {
            if (a.hasArg('a') && !balls[i].containsTagHierarchy(tagStr))
            {
                balls[i].addTopLevelTag(tag);
            }
            else if (balls[i].containsTagHierarchy(tagStr))
            {
                tag = balls[i][tagStr];
            }
            if (a.hasArg("nndetect"))
            {
                setNndetectEnable(tag, a.getAs<std::string>("nndetect") == "TRUE");
            }
            if (a.hasArg("agegroup"))
            {
                setAgeGroup(tag, a.getAs<std::string>("agegroup"));
            }
            if (a.hasArg("category"))
            {
                retypeTo(a.getAs<std::string>("category"), tag, balls[i]);
            }
            if (a.hasArg("addalias"))
            {
                tag->addAlias(a.getAs<std::string>("addalias"));
            }
            if (a.hasArg("removealias"))
            {
                tag->removeAlias(a.getAs<std::string>("removealias"));
            }
            if (a.hasArg("addalias") && a.hasArg("removealias") && a.getAs<std::string>("addalias") == a.getAs<std::string>("removealias"))
            {
                funE();
            }
            if (a.hasArg("clearaliases"))
            {
                tag->getAliases().clear();
            }
            if (a.hasArg("addparent"))
            {
                addParentTo(a.getAs<std::string>("addparent"), tag, balls[i]);
            }
            if (a.hasArg("removeparent"))
            {
                removeParentFrom(a.getAs<std::string>("removeparent"), tag);
            }
            if (a.hasArg("addparent") && a.hasArg("removeparent") && a.getAs<std::string>("addparent") == a.getAs<std::string>("removeparent"))
            {
                funE();
            }
            if (a.hasArg("orphan"))
            {
                while (tag->getParentCount() != 0)
                {
                    tag->removeParent(0);
                }
            }
            if (a.hasArg("addchild"))
            {
                if (balls[i][a.getAs<std::string>("addchild")] != nullptr)
                {
                    tag->addChild(balls[i][a.getAs<std::string>("addchild")]);
                }
            }
            if (a.hasArg("removechild"))
            {
                for (u64 j = 0; j < tag->getChildCount(); j++)
                {
                    if (tag->getChild(j)->getTagString() == a.getAs<std::string>("removechild"))
                    {
                        tag->removeChild(j);
                        break;
                    }
                }
            }
            if (a.hasArg("addchild") && a.hasArg("removechild") && a.getAs<std::string>("addchild") == a.getAs<std::string>("removechild"))
            {
                funE();
            }
            if (a.hasArg("endbloodline"))
            {
                while (tag->getChildCount() != 0)
                {
                    tag->removeChild(0);
                }
            }
            if (a.hasArg('r'))
            {
                if (a.hasArg('a')) funE();
                if (balls[i].containsTagHierarchy(tag->getTagString())) deleteTag(tag, balls[i]);
            }
            if (a.hasArg('q'))
            {
                queryInfo(tag, balls[i]);
            }
            std::vector<char> buf = balls[i].getBuf();
            std::ofstream out(balls[i].getFilename());
            out.write(buf.data(), buf.size());
            out.close();
        }
        tag = nullptr;
    }
    return 0;
}

void mergeFiles(tmc::Bidoof::NdbTagWeb& in, tmc::Bidoof::NdbTagWeb& out,
                std::vector<std::string> filter,
                std::shared_ptr<tmc::Bidoof::NdbTag> dest, bool prompt, bool overwrite)
{
    if (filter.empty())
    {
        if (dest == nullptr)
        {
            for (u64 i = 0; i < in.getTopLevelTagCount(); i++)
            {
                if (out[in[i]->getTagString()] == nullptr)
                {
                    out.addTopLevelTag(in[i]);
                }
                else if (prompt)
                {
                    overwrite = tmc::yesNoPrompt("\nTag already exists.  Overwrite? ");
                }
                if (out[in[i]->getTagString()] != nullptr && overwrite)
                {
                    while (out[in[i]->getTagString()]->getChildCount() != 0)
                    {
                        removeParentFrom(in[i]->getTagString(), out[in[i]->getTagString()]->getChild(0));
                    }
                    out[in[i]->getTagString()] = in[i];
                }
                std::cout << "\nMerged " << i << " out of " << in.getTopLevelTagCount();
            }
        }
        else
        {
            for (u64 i = 0; i < in.getTopLevelTagCount(); i++)
            {
                if (out[in[i]->getTagString()] == nullptr)
                {
                    dest->addChild(in[i]);
                }
                else if (prompt)
                {
                    overwrite = tmc::yesNoPrompt("\nTag already exists.  Overwrite? ");
                }
                if (out[in[i]->getTagString()] != nullptr && overwrite)
                {
                    auto temp = out[in[i]->getTagString()];
                    deleteTag(temp, out);
                    dest->addChild(in[i]);
                }
                std::cout << "\nMerged " << i << " out of " << in.getTopLevelTagCount();
            }
        }
    }
    else
    {
        if (dest == nullptr)
        {
            for (u64 i = 0; i < filter.size(); i++)
            {
                if (in[filter[i]] != nullptr)
                {
                    while (in[filter[i]]->getParentCount() != 0)
                    {
                        removeChildFrom(filter[i], in[filter[i]]->getParent(0));
                    }
                    if (out[filter[i]] == nullptr)
                    {
                        out.addTopLevelTag(in[filter[i]]);
                    }
                    else if (prompt)
                    {
                        overwrite = tmc::yesNoPrompt("\nTag already exists.  Overwrite? ");
                    }
                    if (out[filter[i]] != nullptr && overwrite)
                    {
                        while (out[filter[i]]->getChildCount() != 0)
                        {
                            removeParentFrom(filter[i], out[filter[i]]->getChild(0));
                        }
                        out[filter[i]] = in[filter[i]];
                    }
                }
                std::cout << "\nMerged " << i << " out of " << filter.size();
            }
        }
        else
        {
            for (u64 i = 0; i < filter.size(); i++)
            {
                if (in[filter[i]] != nullptr)
                {
                    while (in[filter[i]]->getParentCount() != 0)
                    {
                        removeChildFrom(filter[i], in[filter[i]]->getParent(0));
                    }
                    if (out[filter[i]] == nullptr)
                    {
                        dest->addChild(in[filter[i]]);
                    }
                    else if (prompt)
                    {
                        overwrite = tmc::yesNoPrompt("\nTag already exists.  Overwrite? ");
                    }
                    if (out[filter[i]] != nullptr && overwrite)
                    {
                        auto temp = out[in[i]->getTagString()];
                        deleteTag(temp, out);
                        dest->addChild(in[filter[i]]);
                    }
                }
                std::cout << "\nMerged " << i << " out of " << filter.size();
            }
        }
    }
}

void funE()
{
    std::cout << "Congratulations!  You did a whole lot of nothing!\n";
}

void printHelp()
{
    std::cout << "Usage:\n"
              << "\ttmcntw-edit\t\t(run in interactive mode)\n\n"
              << "\ttmcntw-edit -t <TAG> [-arq] [--nndetect TRUE|FALSE]"
              << " [--agegroup SAFE|QUESTIONABLE|EXPLICIT]"
              << " [--addalias <ALIAS>] [--removealias <ALIAS>]"
              << " [--clearaliases]"
              << " [--addparent <TAG>] [--removeparent <TAG>]"
              << " [--orphan] [--addchild <TAG>] [--removechild <TAG>]"
              << " [--endbloodline] [--category COPYRIGHT|AUTHOR|"
              << "CHARACTER|SPECIES|GENERAL|META|LORE]"
              << " -- <file1> [file2] ...\t\t(run in script mode)\n\n"
              << "\ttmcntw-edit -l -- <FILE>\t\t(Print file's tag tree)\n\n"
              << "\ttmcntw-edit -m <FILE1> [<FILE2> ...] [-s TAG1 [TAG2 ...]] [-d TAG]"
              << " [[-y|--yes]|[-n|--no]] -- FILE (run in merge mode)\n\n"
              << "\nEdit .tmcntw files and libTmcAsset formats with bundled tmcntw data\n\n"
              << "-t <TAG>\t\t\tOperate on this tag"
              << "-a\t\t\tadd a new tag specified with -t\t"
              << "-r\t\t\tremove tag specified by -t\n"
              << "-q\t\t\tquery info for tag specified by -t\n"
              << "--nndetect <bool>\t\t\tEnable/disable using this tag to enable HL3-licensed DRM\n"
              << "\tExamples: tmcntw-edit -t vaporeon --nndetect TRUE -- balls.tmcntw\n"
              << "\ttmcntw-edit -t shark --nndetect FALSE -- balls.tmcntw\n"
              << "--agegroup <RATING>\t\t\tSet the lowest rating this tag can apply to\n"
              << "\tSAFE = safe for all ages\n\tQUESTIONABLE = 13+\n\tExplicit = 18+\n"
              << "\tExamples: tmcntw-edit -t symmetry --agegroup SAFE -- balls.tmcntw\n"
              << "\ttmcntw-edit -t profanity --agegroup QUESTIONABLE -- balls.tmcntw\n"
              << "\ttmcntw-edit -t balls --agegroup EXPLICIT -- balls.tmcntw\n"
              << "--category <CATEGORY>\t\t\tThe category this tag belongs to\n"
              << "\tExamples: tmcntw-edit -t tmc0m8u570rz --category COPYRIGHT -- balls.tmcntw\n"
              << "\ttmcntw-edit -t melsbacksfriend --category AUTHOR -- balls.tmcntw\n"
              << "\ttmcntw-edit -t ben_the_diphallic_vap --category CHARACTER -- balls.tmcntw\n"
              << "\ttmcntw-edit -t eevee --category SPECIES -- balls.tmcntw\n"
              << "\ttmcntw-edit -t green_background --category GENERAL -- balls.tmcntw\n"
              << "\ttmcntw-edit -t monochrome --category META -- balls.tmcntw\n"
              << "\ttmcntw-edit -t breaking_the_fourth_wall --category LORE -- balls.tmcntw\n"
              << "--addalias <ALIAS>\t\t\tadds a new alias\n"
              << "\tExample: tmcntw-edit -t penis --addalias phallus -- balls.tmcntw"
              << "--removealias <ALIAS>\t\t\tremoves an alias\n"
              << "--clearaliases\t\t\tremoves all aliases\n"
              << "--addparent <PARENT>\t\t\tadds a new parent\n"
              << "\tExample: tmcntw-edit -t penis --addparent genitals -- balls.tmcntw"
              << "--removeparent <PARENT>\t\t\tremoves a parent\n"
              << "--orphan\t\t\tremoves all parents\n"
              << "--addchild <CHILD>\t\t\tadds a new child\n"
              << "\tExample: tmcntw-edit -t penis --addchild animal_penis -- balls.tmcntw"
              << "--removechild <CHILD>\t\t\tremoves a child\n"
              << "--endbloodline\t\t\tremoves all children\n"
              << "-m <FILE1> [FILE2 ...]\t\t\tspecify merge source files\n"
              << "-s TAG1 [TAG2 ...]\t\t\tfilter to specific tags to merge recursively (forward propagating)\n"
              << "-d TAG\t\t\tSpecify destination parent\n\tOUTPUT MUST EXIST IF THIS IS SPECIFIED!!!\n"
              << "-y|--yes\t\t\tOverwrite on conflicts\n"
              << "\tThis arg is mutually exclusive with -n|--no\n"
              << "\tIf neither is specified, the user is prompted\n"
              << "\t(bad for unattended runs)\n"
              << "-n|--no\t\t\tSkip on conflicts\n"
              << "\tThis arg is mutually exclusive with -y|--yes\n"
              << "\tIf neither is specified, the user is prompted\n"
              << "\t(bad for unattended runs)\n"
              << "-h|--help\t\t\tprint this text\n"
              << "--\t\t\tTreat all arguments after this one as file names.\n"
              << "If file does not exist, it is created.\n";
}

void printTree(tmc::Bidoof::NdbTagWeb& ntw)
{
    std::cout << "\n" << ntw.getFilename();
    for (u64 i = 0; i < ntw.getTopLevelTagCount(); i++)
    {
        recursiveTagPrint(ntw[i]);
    }
    std::cout << "\n";
}

void recursiveTagPrint(std::shared_ptr<tmc::Bidoof::NdbTag>& nt, u64 depth)
{
    std::cout << "\n";
    std::cout << "|";
    for (u64 i = 0; i < depth; i++)
    {
        std::cout << " |";
    }
    std::cout << "-" << nt->getTagString();
    for (u64 i = 0; i < nt->getChildCount(); i++)
    {
        recursiveTagPrint(nt->getChild(i), depth + 1);
    }

}

void removeChildFrom(std::string tagStr, std::shared_ptr<tmc::Bidoof::NdbTag>& nt)
{
    for (u64 j = 0; j < nt->getChildCount(); j++)
    {
        if (nt->getChild(j)->getTagString() == tagStr)
        {
            nt->removeChild(j);
            break;
        }
    }
}

void removeParentFrom(std::string tagStr, std::shared_ptr<tmc::Bidoof::NdbTag>& nt)
{
    for (u64 j = 0; j < nt->getParentCount(); j++)
    {
        if (nt->getParent(j)->getTagString() == tagStr)
        {
            nt->removeParent(j);
            break;
        }
    }
}

void deleteTag(std::shared_ptr<tmc::Bidoof::NdbTag>& nt, tmc::Bidoof::NdbTagWeb& ntw)
{
    while (nt->getParentCount() != 0)
    {
        removeChildFrom(nt->getTagString(), nt->getParent(0));
    }
    while (nt->getChildCount() != 0)
    {
        removeParentFrom(nt->getTagString(), nt->getChild(0));
    }
    for (u64 i = 0; i < ntw.getTopLevelTagCount(); i++)
    {
        if (ntw[i]->getTagString() == nt->getTagString())
        {
            ntw.removeTopLevelTag(i);
            break;
        }
    }
}

void retypeTo(std::string arg, std::shared_ptr<tmc::Bidoof::NdbTag>& nt, tmc::Bidoof::NdbTagWeb& ntw)
{
    std::shared_ptr<tmc::Bidoof::NdbTag> temp = nullptr;
    if (arg == "COPYRIGHT")
    {
        temp = tmc::Bidoof::ndb_tag_cast<tmc::Bidoof::CopywrongTag>(nt);
    }
    else if (arg == "AUTHOR")
    {
        temp = tmc::Bidoof::ndb_tag_cast<tmc::Bidoof::AuthorTag>(nt);
    }
    else if (arg == "CHARACTER")
    {
        temp = tmc::Bidoof::ndb_tag_cast<tmc::Bidoof::CharTag>(nt);
    }
    else if (arg == "SPECIES")
    {
        temp = tmc::Bidoof::ndb_tag_cast<tmc::Bidoof::SpeciesTag>(nt);
    }
    else if (arg == "GENERAL")
    {
        temp = tmc::Bidoof::ndb_tag_cast<tmc::Bidoof::GeneralTag>(nt);
    }
    else if (arg == "META")
    {
        temp = tmc::Bidoof::ndb_tag_cast<tmc::Bidoof::MetaTag>(nt);
    }
    else if (arg == "LORE")
    {
        temp = tmc::Bidoof::ndb_tag_cast<tmc::Bidoof::LoreTag>(nt);
    }
    deleteTag(nt, ntw);
    if (temp->getParentCount() == 0)
    {
        ntw.addTopLevelTag(temp);
    }
    nt = temp;
}

void addParentTo(std::string arg, std::shared_ptr<tmc::Bidoof::NdbTag>& nt, tmc::Bidoof::NdbTagWeb& ntw)
{
    if (ntw[arg] != nullptr) nt->addParent(ntw[arg]);
}

void addChildTo(std::string arg, std::shared_ptr<tmc::Bidoof::NdbTag>& nt, tmc::Bidoof::NdbTagWeb& ntw)
{
    if (ntw[arg] != nullptr) nt->addChild(ntw[arg]);
}

void orphan(std::shared_ptr<tmc::Bidoof::NdbTag>& nt)
{
    while(nt->getParentCount() != 0)
    {
        nt->removeParent(0);
    }
}

void endBloodline(std::shared_ptr<tmc::Bidoof::NdbTag>& nt)
{
    while(nt->getChildCount() != 0)
    {
        nt->removeChild(0);
    }
}

void addAliasTo(std::string arg, std::shared_ptr<tmc::Bidoof::NdbTag>& nt)
{
    nt->addAlias(arg);
}

void removeAliasFrom(std::string arg, std::shared_ptr<tmc::Bidoof::NdbTag>& nt)
{
    nt->removeAlias(arg);
}

void clearAliases(std::shared_ptr<tmc::Bidoof::NdbTag>& nt)
{
    nt->getAliases().clear();
}

void setNndetectEnable(std::shared_ptr<tmc::Bidoof::NdbTag>& nt, bool enabled)
{
    u8 am = nt->getAccessModifier() & 3;
    if (enabled) am |= 0x80;
    nt->setAccessModifier(static_cast<tmc::Bidoof::NdbTag::AxceModYum>(am));
}

void setAgeGroup(std::shared_ptr<tmc::Bidoof::NdbTag>& nt, std::string ageGroup)
{
    u8 am = nt->getAccessModifier() & 0x80;
    if (ageGroup == "QUESTIONABLE") am |= tmc::Bidoof::NdbTag::RQ;
    else if (ageGroup == "EXPLICIT") am |= tmc::Bidoof::NdbTag::RE;
    nt->setAccessModifier(static_cast<tmc::Bidoof::NdbTag::AxceModYum>(am));
}

void queryInfo(std::shared_ptr<tmc::Bidoof::NdbTag>& nt, tmc::Bidoof::NdbTagWeb& ntw)
{
    if (ntw[nt->getTagString()] == nullptr)
    {
        std::cout << "Tag doesn't exist\n";
    }
    else
    {
        std::cout << "Category: ";
        if (nt->getCategory() == tmc::Bidoof::NdbTag::COPYRIGHT)
        {
            std::cout << "Copyright\n";
        }
        else if (nt->getCategory() == tmc::Bidoof::NdbTag::AUTHOR)
        {
            std::cout << "Author\n";
        }
        else if (nt->getCategory() == tmc::Bidoof::NdbTag::CHARACTER)
        {
            std::cout << "Character\n";
        }
        else if (nt->getCategory() == tmc::Bidoof::NdbTag::SPECIES)
        {
            std::cout << "Species\n";
        }
        else if (nt->getCategory() == tmc::Bidoof::NdbTag::GENERAL)
        {
            std::cout << "General\n";
        }
        else if (nt->getCategory() == tmc::Bidoof::NdbTag::META)
        {
            std::cout << "Meta\n";
        }
        else if (nt->getCategory() == tmc::Bidoof::NdbTag::LORE)
        {
            std::cout << "Lore\n";
        }
        else if (nt->getCategory() & tmc::Bidoof::NdbTag::RESERVED)
        {
            std::cout << "Reserved\n";
        }
        std::cout << "\nParents:\n";
        for (u64 j = 0; j < nt->getParentCount(); j++)
        {
            std::cout << nt->getParent(j)->getTagString() << "\n";
        }
        std::cout << "\nChildren:\n";
        for (u64 j = 0; j < nt->getChildCount(); j++)
        {
            std::cout << nt->getChild(j)->getTagString() << "\n";
        }
        std::cout << "\nAliases:\n";
        for (u64 j = 0; j < nt->getAliases().size(); j++)
        {
            std::cout << nt->getAliases()[j] << "\n";
        }
        std::cout << "\nDRM Protected: ";
        std::cout << (((nt->getAccessModifier() & 0b11111100) == 0x80) ? "TRUE\n" : "FALSE\n");
        std::cout << "\nAge rating: ";
        if ((nt->getAccessModifier() & 0b11) == 0)
        {
            std::cout << "All ages\n";
        }
        else if ((nt->getAccessModifier() & 0b11) == 1)
        {
            std::cout << "13+\n";
        }
        else
        {
            std::cout << "18+\n";
        }
    }
}
