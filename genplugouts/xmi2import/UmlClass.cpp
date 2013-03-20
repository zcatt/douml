
#include "UmlClass.h"
#include "UmlItem.h"
#include "Token.h"
#include "FileIn.h"
#include "UmlTypeSpec.h"

#include "UmlAttribute.h"
#include "UmlRelation.h"
#include "UmlOperation.h"
#include "UmlNcRelation.h"
#include "Association.h"
#include "UmlCom.h"
#include "Binding.h"
#include "ClassInstance.h"
#include "CppSettings.h"
#include "UmlClassView.h"
#include "UmlPackage.h"
//Added by qt3to4:
#include "misc/mystr.h"
#include <Q3ValueList>

UmlItem * UmlClass::container(anItemKind kind, Token & token, FileIn & in)
{
    switch (kind) {
    case aClass:
    case anAttribute:
    case aRelation:
    case anOperation:
        return this;

    default:
        return parent()->container(kind, token, in);
    }

}

void UmlClass::init()
{
    declareFct("ownedmember", "uml:Class", &importIt);
    declareFct("packagedelement", "uml:Class", &importIt);
    declareFct("nestedclassifier", "uml:Class", &importIt);

    declareFct("ownedmember", "uml:AssociationClass", &importIt);
    declareFct("packagedelement", "uml:AssociationClass", &importIt);
    declareFct("nestedclassifier", "uml:AssociationClass", &importIt);

    declareFct("ownedmember", "uml:Interface", &importIt);
    declareFct("packagedelement", "uml:Interface", &importIt);
    declareFct("nestedclassifier", "uml:Interface", &importIt);

    declareFct("ownedmember", "uml:Enumeration", &importIt);
    declareFct("packagedelement", "uml:Enumeration", &importIt);
    declareFct("nestedclassifier", "uml:Enumeration", &importIt);

    declareFct("ownedmember", "uml:Actor", &importIt);
    declareFct("packagedelement", "uml:Actor", &importIt);
    declareFct("nestedclassifier", "uml:Actor", &importIt);

    declareFct("ownedmember", "uml:Stereotype", &importIt);
    declareFct("packagedelement", "uml:Stereotype", &importIt);

    //

    declareFct("ownedmember", "uml:PrimitiveType", &importPrimitiveType);
    declareFct("packagedelement", "uml:PrimitiveType", &importPrimitiveType);
    declareFct("ownedmember", "uml:DataType", &importPrimitiveType);
    declareFct("packagedelement", "uml:DataType", &importPrimitiveType);

    //

    UmlAttribute::init();
    UmlRelation::init();
    UmlOperation::init();
    ClassInstance::init();
}

void UmlClass::importIt(FileIn & in, Token & token, UmlItem * where)
{
    where = where->container(aClass, token, in);	// can't be null

    WrapperStr s = token.valueOf("name");

    if (s.isEmpty()) {
        static unsigned n = 0;

        s.sprintf("anonymous_%u", ++n);
    }
    else
        s = legalName(s);

    UmlClass * cl = create(where, s);
    Association * assocclass = 0;
    bool stereotype = FALSE;

    if (cl == 0)
        in.error("cannot create classe '" + s +
                 "' in '" + where->name() + "'");

    cl->addItem(token.xmiId(), in);

    do
        where = where->parent();

    while (where->kind() != aPackage);

    if (where->stereotype() == "profile")
        cl->set_PropertyValue("xmiId", token.xmiId());

    if (token.xmiType() == "uml:Actor")
        cl->set_Stereotype("actor");
    else if (token.xmiType() == "uml:Interface")
        cl->set_Stereotype("interface");
    else if (token.xmiType() == "uml:Enumeration")
        cl->set_Stereotype("enum");
    else if (token.xmiType() == "uml:Stereotype") {
        cl->set_Stereotype("stereotype");
        NumberOf -= 1;
        NumberOfStereotype += 1;
        stereotype = TRUE;
    }
    else if (token.xmiType() == "uml:AssociationClass") {
        assocclass = &Association::get(token.xmiId(), token.valueOf("name"));
        assocclass->set_class_association();
    }

    cl->setVisibility(token.valueOf("visibility"));

    if (token.valueOf("isabstract") == "true")
        cl->set_isAbstract(TRUE);

    if (token.valueOf("isactive") == "true")
        cl->set_isActive(TRUE);

    if (! token.closed()) {
        WrapperStr k = token.what();
        const char * kstr = k;
        WrapperStr assocclass_ref1;
        WrapperStr assocclass_ref2;

        while (in.read(), !token.close(kstr)) {
            s = token.what();

            if ((s == "ownedtemplatesignature") &&
                ((token.xmiType() == "uml:TemplateSignature") ||
                 (token.xmiType() == "uml:RedefinableTemplateSignature")))
                cl->readFormal(in, token);
            else if ((s == "templatebinding") &&
                     (token.xmiType() == "uml:TemplateBinding")) {
                Binding::import(in, token, cl);
            }
            else if ((assocclass != 0) && (s == "memberend")) {
                if (assocclass_ref1.isEmpty())
                    assocclass_ref1 = token.xmiIdref();
                else
                    assocclass_ref2 = token.xmiIdref();

                if (! token.closed())
                    in.finish(s);
            }
            else if ((assocclass != 0) &&
                     (s == "ownedend") &&
                     (token.xmiType() == "uml:Property"))
                assocclass->import(in, token);
            else if (s == "ownedrule")
                cl->set_Constraint(UmlItem::readConstraint(in, token));
            else if (stereotype &&
                     (s == "icon") &&
                     (token.xmiType() == "uml:Image")) {
                WrapperStr path = token.valueOf("location");

                if (! path.isEmpty())
                    cl->set_PropertyValue("stereotypeIconPath", path);

                if (! token.closed())
                    in.finish(s);
            }
            else
                cl->UmlItem::import(in, token);
        }
    }

    cl->unload(TRUE, FALSE);
}

void UmlClass::importPrimitiveType(FileIn & in, Token & token, UmlItem *)
{
    WrapperStr id = token.xmiId();
    UmlTypeSpec t;

    t.explicit_type = token.valueOf("name");

    if (FromBouml) {
        if (! token.closed()) {
            BooL dummy;

            in.read(); 	// <xmi:Extension extender="Bouml">
            in.readWord(FALSE, dummy);	// <
            in.readWord(FALSE, dummy);	// basedOn
            t.type = dynamic_cast<UmlClass *>(All[in.readWord(FALSE, dummy)]);

            if (t.type != 0)
                // forget modifiers
                t.explicit_type = "";

            in.readWord(FALSE, dummy);	// /
            in.readWord(FALSE, dummy);	// >
            in.read(); 	// </xmi:Extension>
            in.read();	// end of token
        }
    }
    else if (! token.closed())
        in.finish(token.what());

    if (!id.isEmpty())
        PrimitiveTypes[id] = t;
}

void UmlClass::generalizeDependRealize(UmlItem * target, FileIn & in, int context, WrapperStr label, WrapperStr constraint)
{
    static const struct {
        aRelationKind rk;
        const char * err;
    } r[] = {
        { aGeneralisation, "cannot create generalization from '" },
        { aDependency, "cannot create dependency from '" },
        { aRealization, "cannot create realization from '" },
        { aDependency, "cannot create usage from '" },
        { aDependency, "cannot create import from '" }
    };
    UmlItem * rel;

    if (target->kind() == aClass)
        rel = UmlRelation::create(r[context].rk, this, (UmlClass *) target);
    else
        rel = UmlNcRelation::create(r[context].rk, this, target);

    if (rel == 0)
        in.warning(r[context].err + name() + "' to '" + target->name() + "'");
    else {
        if (! label.isEmpty())
            rel->set_Name(label);

        if (! constraint.isEmpty() && (target->kind() == aClass))
            ((UmlRelation *) rel)->set_Constraint(constraint);
    }
}

void UmlClass::solveGeneralizationDependencyRealization(int context, WrapperStr idref, WrapperStr label, WrapperStr constraint)
{
    QMap<QString, UmlItem *>::Iterator it = All.find(idref);

    if (it != All.end()) {
        static const struct {
            aRelationKind rk;
            const char * err;
        } r[] = {
            { aGeneralisation, "cannot create generalization from '" },
            { aDependency, "cannot create dependency from '" },
            { aRealization, "cannot create realization from '" },
            { aDependency, "cannot create usage from '" },
            { aDependency, "cannot create import from '" }
        };
        UmlItem * target = *it;
        UmlItem * rel;

        if (target->kind() == aClass)
            rel = UmlRelation::create(r[context].rk, this, (UmlClass *) target);
        else
            rel = UmlNcRelation::create(r[context].rk, this, target);

        if (rel == 0)
            UmlCom::trace(r[context].err + name() + "' to '" + target->name() + "'<br>");
        else {
            if (!label.isEmpty())
                rel->set_Name(label);

            if (!constraint.isEmpty() && (target->kind() == aClass))
                ((UmlRelation *) rel)->set_Constraint(constraint);

            if (context == 3)
                rel->set_Stereotype("use");
        }
    }
    else if (!FileIn::isBypassedId(idref))
        UmlCom::trace("relation : unknown target reference '" + idref + "'<br>");
}

UmlClass * UmlClass::signature(WrapperStr id)
{
    QMap<QString, UmlClass *>::Iterator iter = signatures.find(id);

    return (iter == signatures.end()) ? 0 : *iter;
}

int UmlClass::formalRank(WrapperStr id)
{
    int r = formalsId.findIndex(id);

    if ((r == -1) && !FileIn::isBypassedId(QString::number(r)))
        UmlCom::trace("unknown template formal reference '" + id + "'<br>");

    return r;
}

bool UmlClass::bind(UmlClass * tmpl)
{
    const Q3PtrVector<UmlItem> ch = children();
    unsigned int n = ch.size();
    int i;

    for (i = 0; i != (int) n; i += 1) {
        if ((ch[i]->kind() == aRelation) &&
            (((UmlRelation *) ch[i])->roleType() == tmpl)) {
            switch (((UmlRelation *) ch[i])->relationKind()) {
            case aRealization:
                ((UmlRelation *) ch[i])->set_Stereotype("bind");

                // no break
            case aGeneralisation:
                return TRUE;

            default:
                break;
            }
        }
    }

    // add realization
    UmlRelation * r =
        UmlRelation::create(aRealization, this, tmpl);

    if (r == 0) {
        UmlCom::trace("class reference '" + id() +
                      "' can't realize class reference '" + tmpl->id() + "'<br>");

        return FALSE;
    }

    r->set_Stereotype("bind");
    return TRUE;
}

void UmlClass::extend(WrapperStr mcl)
{
    if (parent()->parent()->kind() != aPackage)
        return;

    int index = mcl.find('#');

    if (index == -1)
        return;

    WrapperStr path = mcl.left(index);
    const char * defltpath0 = "http://schema.omg.org/spec/UML/2.0/uml.xml";
    const char * defltpath1 = "http://schema.omg.org/spec/UML/2.1/uml.xml";
    bool dflt = ((path == defltpath0) || (path == defltpath1));

    mcl = mcl.mid(index + 1);

    static Q3PtrList<UmlClass> metaclasses;

    Q3PtrListIterator<UmlClass> it(metaclasses);
    UmlClass * metacl = UmlClass::get(mcl, 0);
    WrapperStr s;

    if ((metacl == 0) ||
        (metacl->stereotype() != "metaclass") ||
        !((dflt) ? (!metacl->propertyValue("metaclassPath", s) ||
                    (s == defltpath0) ||
                    (s == defltpath1))
          : (metacl->propertyValue("metaclassPath", s) &&
             (path == s)))) {
        metacl = 0;

        if (dflt) {
            for (; (metacl = it.current()) != 0; ++it) {
                if (!strcmp(mcl, metacl->name()) &&
                    (!metacl->propertyValue("metaclassPath", s) ||
                     (s == defltpath0) ||
                     (s == defltpath1)))
                    break;
            }
        }
        else {
            for (; (metacl = it.current()) != 0; ++it) {
                if (!strcmp(mcl, metacl->name()) &&
                    metacl->propertyValue("metaclassPath", s) &&
                    (path == s))
                    break;
            }
        }

        if (metacl == 0) {
            metacl = addMetaclass(mcl, (dflt) ? 0 : (const char *)path); //[rageek] different types for ? :
            metaclasses.append(metacl);
        }
    }

    UmlRelation::create(aDirectionalAssociation, this, metacl);
}

bool UmlClass::isAppliedStereotype(Token & tk, WrapperStr & prof_st, Q3ValueList<WrapperStr> & base_v)
{
    static Q3Dict<WrapperStr> stereotypes;
    static Q3Dict<Q3ValueList<WrapperStr> > bases;

    WrapperStr s = tk.what();
    WrapperStr * st = stereotypes[s];

    if (st != 0) {
        prof_st = *st;
        base_v = *bases[s];
        return TRUE;
    }

    base_v.clear();

    if (tk.xmiType().isEmpty() && (getFct(tk) == 0))  {
        int index = s.find(':');

        if ((index != -1) &&
            ((index != 3) || ((s.left(3) != "uml") && (s.left(3) != "xmi")))) {
            UmlClass * cl = findStereotype(s, FALSE);

            if (cl != 0) {
                const Q3PtrVector<UmlItem> ch = cl->children();
                unsigned n = ch.size();

                for (unsigned i = 0; i != n; i += 1) {
                    UmlItem * x = ch[i];

                    if ((x->kind() == aRelation) &&
                        (((UmlRelation *) x)->relationKind() == aDirectionalAssociation) &&
                        (((UmlRelation *) x)->roleType()->stereotype() == "metaclass"))
                        base_v.append("base_" + ((UmlRelation *) x)->roleType()->name().lower());
                }

                if (base_v.isEmpty())
                    base_v.append("base_element");

                prof_st = cl->parent()->parent()->name() + ":" + cl->name();
                stereotypes.insert(s, new WrapperStr(prof_st));
                bases.insert(s, new Q3ValueList<WrapperStr>(base_v));
                return TRUE;
            }
        }
    }

    return FALSE;
}

bool UmlClass::isPrimitiveType(Token & token, UmlTypeSpec & ts)
{
    if (token.xmiType() != "uml:PrimitiveType")
        return FALSE;

    WrapperStr href = token.valueOf("href");
    int index;

    if (href.isEmpty() || ((index = href.find('#')) == -1))
        return FALSE;

    ts.explicit_type = href.mid(index + 1);

    if ((CppSettings::type(ts.explicit_type) == ts.explicit_type) &&
        CppSettings::umlType(ts.explicit_type).isEmpty()) {
        // not defined
        href = ts.explicit_type.lower();

        if (href == "integer")
            ts.explicit_type = "int";
        else if (href == "boolean")
            ts.explicit_type = "bool";
        else if (href == "string")
            ts.explicit_type = "string";
        else if (href == "unlimitednatural")
            ts.explicit_type = "long";
    }

    return TRUE;
}

void UmlClass::readFormal(FileIn & in, Token & token)
{
    if (! token.closed()) {
        signatures[token.xmiId()] = this;

        WrapperStr k = token.what();
        const char * kstr = k;
        unsigned int rank = 0;

        while (in.read(), !token.close(kstr)) {
            WrapperStr s = token.what();

            if (s == "parameter") {
                // useless
                if (! token.closed())
                    in.finish(token.what());
            }
            else if ((s == "ownedparameter") &&
                     (token.xmiType() == "uml:ClassifierTemplateParameter")) {
                WrapperStr idparam = token.xmiId();
                WrapperStr pname = token.valueOf("name");	// at least for VP
                WrapperStr value;

                if (! token.closed()) {
                    while (in.read(), !token.close("ownedparameter")) {
                        s = token.what();

                        if ((s == "ownedparameteredelement") ||
                            (s == "ownedelement")) {
                            s = token.valueOf("name");

                            if (! s.isEmpty())
                                pname = s;
                        }
                        else if (s == "defaultvalue")
                            value = token.valueOf("value");

                        if (! token.closed())
                            in.finish(token.what());
                    }
                }

                if (! pname.isEmpty()) {
                    UmlFormalParameter f(pname, value);

                    addFormal(rank++, f);
                    formalsId.append(idparam);
                }
            }
            else if (! token.closed())
                in.finish(token.what());
        }
    }
}

UmlClass * UmlClass::addMetaclass(WrapperStr mclname, const char * mclpath)
{
    UmlPackage * pack = (UmlPackage *) parent()->parent();	// is a package
    const Q3PtrVector<UmlItem> ch = pack->children();
    unsigned n = ch.size();
    UmlClass * r = 0;

    for (unsigned i = 0; i != n; i += 1) {
        UmlItem * x = ch[i];

        if ((x->kind() == aClassView) &&
            !strncmp(x->name(), "meta classes", 12) &&
            ((r = UmlClass::create(x, mclname)) != 0))
            break;
    }

    if (r == 0) {
        WrapperStr s = "meta classes";
        UmlItem * v = 0;

        while ((v = UmlClassView::create(pack, s)) == 0)
            s += "_";

        r = UmlClass::create(v, mclname);
    }

    r->set_Stereotype("metaclass");

    if (mclpath != 0)
        r->set_PropertyValue("metaclassPath", mclpath);

    return r;
}

int UmlClass::NumberOf;

int UmlClass::NumberOfStereotype;

//associate the class owning the template signature with the signature id
QMap<QString, UmlClass *> UmlClass::signatures;

