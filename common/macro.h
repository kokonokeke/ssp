#ifndef _MACRO_H
#define _MACRO_H

#define GETSETVAR(type, name)\
        private:\
            type name##_;\
        public:\
            const type& name() const {\
                return name##_;\
            }\
            void set_##name(const type& value) {\
                name##_ = value;\
            }
#define GETSETSTR(size, name)\
        private:\
            char name##_[size];\
        public:\
            const char *name() const {\
                return name##_;\
            }\
            void set_##name(const char *value) {\
                strncpy(name##_, value, size);\
            }

#endif