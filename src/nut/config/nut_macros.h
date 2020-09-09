#ifndef NUT_MACROS_H
#define NUT_MACROS_H


#define NUT_INFO(type, name, value)                                            \
    Q_CLASSINFO(__nut_NAME_PERFIX type #name #value,                           \
                type "\n" #name "\n" #value)

#define NUT_INFO_STRING(type, name, value)                                     \
    Q_CLASSINFO(__nut_NAME_PERFIX type #name #value,                           \
                type "\n" #name "\n" value)

#define NUT_FIELD_PREFIX
#define NUT_FIELD_POSTFIX Field

// Database
#define NUT_DB_VERSION(version)  \
    NUT_INFO(__nut_DB_VERSION, version, 0)

#define NUT_DECLARE_TABLE(type, name)                                          \
    NUT_INFO(__nut_TABLE, type, name)                                          \
    Q_PROPERTY(NUT_WRAP_NAMESPACE(TableSet<type>) name READ name)              \
    NUT_WRAP_NAMESPACE(TableSet<type>) *m_##name;                              \
    public:                                                                    \
    static const type *_##name;                                                \
    NUT_WRAP_NAMESPACE(TableSet<type>) *name() const                           \
    { return m_##name; }                                                       \
    private:

//Table
#define NUT_DECLARE_FIELD(type, name, read, write)                             \
    Q_PROPERTY(type name READ read WRITE write)                                \
    NUT_INFO(__nut_FIELD, name, 0)                                             \
    type m_##name;                                                             \
public:                                                                        \
    static NUT_WRAP_NAMESPACE(FieldPhrase<type>)& name ## Field(){             \
        static NUT_WRAP_NAMESPACE(FieldPhrase<type>) f =                       \
                NUT_WRAP_NAMESPACE(FieldPhrase<type>)                          \
                        (staticMetaObject.className(), #name);                 \
        return f;                                                              \
    }                                                                          \
    type read() const{                                                         \
        return m_##name;                                                       \
    }                                                                          \
    void write(type name){                                                     \
        m_##name = name;                                                       \
        propertyChanged(QString::fromUtf8(#name));                             \
    }

#define NUT_FOREIGN_KEY(type, keytype, name, read, write)                      \
    Q_PROPERTY(Nut::Row<type> name READ read WRITE write)                      \
    NUT_DECLARE_FIELD(keytype, name##Id, read##Id, write##Id)                  \
    NUT_INFO(__nut_FOREIGN_KEY, name, type)                                    \
    Nut::Row<type> m_##name;                                                   \
public slots:                                                                  \
    Nut::Row<type> read() const { return m_##name ; }                          \
    Q_INVOKABLE void write(Nut::Row<type> name){                               \
        m_##name = name;                                                       \
    }

#define NUT_FOREIGN_KEY_DECLARE(type, keytype, name, read, write)              \
    NUT_INFO(__nut_FIELD, name##Id, 0)                                         \
    NUT_INFO(__nut_FOREIGN_KEY, name, type)                                    \
    Nut::Row<type> m_##name;                                                   \
    keytype m_##name##Id;                                                      \
    Q_PROPERTY(Nut::Row<type> name READ read WRITE write)                      \
    Q_PROPERTY(keytype name##Id READ read##Id WRITE write##Id)                 \
public:                                                                        \
    Nut::Row<type> read() const;                                               \
    keytype read##Id() const;                                                  \
    static NUT_WRAP_NAMESPACE(FieldPhrase<keytype>)& name##Id ## Field(){      \
        static NUT_WRAP_NAMESPACE(FieldPhrase<keytype>) f =                    \
                NUT_WRAP_NAMESPACE(FieldPhrase<keytype>)                       \
                        (staticMetaObject.className(), #name "Id");            \
        return f;                                                              \
    }                                                                          \
public slots:                                                                  \
    void write(Nut::Row<type> name);                                           \
    void write##Id(keytype name##Id);

#define NUT_FOREIGN_KEY_IMPLEMENT(class, type, keytype, name, read, write)     \
    Nut::Row<type> class::read() const { return m_##name ; }                   \
    void class::write(Nut::Row<type> name){                                    \
        propertyChanged(QStringLiteral(QT_STRINGIFY2(name##Id)));              \
        m_##name = name;                                                       \
        m_##name##Id = name->primaryValue().value<keytype>();                  \
    }                                                                          \
    keytype class::read##Id() const{                                           \
        if (m_##name)                                                          \
            return m_##name->primaryValue().value<keytype>();                  \
        return m_##name##Id;                                                   \
    }                                                                          \
    void class::write##Id(keytype name##Id){                                   \
        m_##name##Id = name##Id;                                               \
        m_##name = nullptr;                                                    \
        propertyChanged(QStringLiteral(QT_STRINGIFY2(name##Id)));              \
    }


#define NUT_DECLARE_CHILD_TABLE(type, n)                                       \
    private:                                                                   \
        NUT_WRAP_NAMESPACE(TableSet)<type> *m_##n;                             \
    public:                                                                    \
        static type *n##Table();                                               \
        NUT_WRAP_NAMESPACE(TableSet)<type> *n();

#define NUT_IMPLEMENT_CHILD_TABLE(class, type, n)                              \
    type *class::n##Table(){                                                   \
        static auto f = new type();                                            \
        return f;                                                              \
    }                                                                          \
    NUT_WRAP_NAMESPACE(TableSet)<type> *class::n(){                            \
        return m_##n;                                                          \
    }

#define NUT_FIELD(name)                     NUT_INFO(__nut_FIELD, name, 0)
#define NUT_PRIMARY_KEY(x)                  NUT_INFO(__nut_PRIMARY_KEY,  x, 0) \
    public:                                                                    \
    QVariant primaryValue() const override {                                   \
        return property(#x);                                                   \
    }                                                                          \
    void setPrimaryValue(const QVariant &value) override {                     \
        setProperty(#x, value);                                                \
    }                                                                          \
    private:


#define NUT_AUTO_INCREMENT(x)               NUT_INFO(__nut_AUTO_INCREMENT, x, 0)
#define NUT_PRIMARY_AUTO_INCREMENT(x)       NUT_INFO(__nut_PRIMARY_KEY_AI, x, 0)\
            NUT_PRIMARY_KEY(x) NUT_AUTO_INCREMENT(x)
#define NUT_DISPLAY_NAME(field, name)       NUT_INFO(__nut_DISPLAY, field, name)
#define NUT_UNIQUE(x)                       NUT_INFO(__nut_UNIQUE, x, 0)
#define NUT_LEN(field, len)                 NUT_INFO(__nut_LEN, field, len)
#define NUT_DEFAULT_VALUE(x, n)             NUT_INFO(__nut_DEFAULT_VALUE, x, n)
#define NUT_NOT_NULL(x)                     NUT_INFO(__nut_NOT_NULL, x, 1)
#define NUT_INDEX(name, field, order)

#endif // NUT_MACROS_H
