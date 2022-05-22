#include "api.hh"
#include "rules.hh"

#include <iomanip>

// from api.cc
extern Api* api;

constexpr auto COMMA = ", ";

/// Decodes a UTF-8 string to a list of 32 bit unicode codepoints. Ignores
/// erroneous characters.
/// Imported from prologin2016
static std::u32string utf8_decode(std::string_view s)
{
    std::u32string ret;
    size_t i = 0;
    size_t size = s.size();

    while (i < size)
    {
        if ((s[i] & 0x80) == 0)
        {
            ret.push_back(s[i++]);
        }
        else if ((s[i] & 0xe0) == 0xc0 && (i + 1) < size &&
                 (s[i + 1] & 0xc0) == 0x80)
        {
            ret.push_back(((s[i] & 0x1f) << 6) | (s[i + 1] & 0x3f));
            i += 2;
        }
        else if ((s[i] & 0xf0) == 0xe0 && (i + 2) < size &&
                 (s[i + 1] & 0xc0) == 0x80 && (s[i + 2] & 0xc0) == 0x80)
        {
            ret.push_back(((s[i] & 0x0f) << 12) | ((s[i + 1] & 0x3f) << 6) |
                          (s[i + 2] & 0x3f));
            i += 3;
        }
        else if ((s[i] & 0xf8) == 0xf0 && (i + 3) < size &&
                 (s[i + 1] & 0xc0) == 0x80 && (s[i + 2] & 0xc0) == 0x80 &&
                 (s[i + 1] & 0xc0) == 0x80)
        {
            ret.push_back(((s[i] & 0x07) << 18) | ((s[i + 1] & 0x3f) << 12) |
                          ((s[i + 2] & 0x3f) << 6) | (s[i + 3] & 0x3f));
            i += 4;
        }
        else
        {
            i++;
        }
    }

    return ret;
}

/// Dump a JSON-escaped UTF-8 string
/// Imported from prologin2016
static void dump_string(std::ostream& ss, std::string_view s)
{
    /*
     * RFC4627, 2.5:
     * All Unicode characters may be placed within the quotation marks except
     * for the characters that must be escaped: quotation mark, reverse solidus,
     * and the control characters (U+0000 through U+001F).
     */
    std::ios state(nullptr);
    state.copyfmt(ss);
    std::u32string utf32 = utf8_decode(s);
    ss << "\"";
    for (char32_t& c : utf32)
    {
        if (c == u'"')
        {
            ss << "\\\"";
        }
        else if (c == u'\\')
        {
            ss << "\\\\";
        }
        else if (u'\u0020' <= c && c <= u'\u007E')
        {
            // printable ASCII
            ss << static_cast<char>(c);
        }
        else if (c > u'\uFFFF')
        {
            // surrogate pair
            // http://unicode.org/faq/utf_bom.html#utf16-2
            const unsigned s = c - 0x010000;
            const unsigned lead = (s >> 10) + 0xD800;
            const unsigned trail = (s & 0x3FF) + 0xDC00;
            ss << "\\u" << std::hex << std::setfill('0') << std::setw(4)
               << lead;
            ss.copyfmt(state);
            ss << "\\u" << std::hex << std::setfill('0') << std::setw(4)
               << trail;
            ss.copyfmt(state);
        }
        else
        {
            ss << "\\u" << std::hex << std::setfill('0') << std::setw(4) << c;
            ss.copyfmt(state);
        }
    }
    ss << '"';
}

struct Str
{
    Str(std::string_view str)
        : str(str)
    {
    }

    std::string_view str;
};

static std::ostream& operator<<(std::ostream& ss, Str str)
{
    dump_string(ss, str.str);

    return ss;
}

// Enums.
// ===========================================================================

// Basic structs.
// ===========================================================================

template <typename V> struct KV
{
    KV(std::string_view key, const V& value)
        : key(key)
        , value(value)
    {
    }

    std::string_view key;
    const V& value;
};

template <typename V>
static std::ostream& operator<<(std::ostream& ss, KV<V> kv)
{
    return ss << Str(kv.key) << ": " << kv.value;
}

template <> std::ostream& operator<<(std::ostream& ss, KV<bool> kv)
{
    return ss << Str(kv.key) << ": " << (kv.value ? "true" : "false");
}

template <typename T> struct Vec
{
    Vec(const std::vector<T>& vec)
        : vec(vec)
    {
    }

    const std::vector<T>& vec;
};

static std::ostream& operator<<(std::ostream& ss, position pos)
{
    return ss << '{' << KV{"colonne", pos.colonne} << ", " << KV{"ligne", pos.ligne} << ", " << KV{"niveau", pos.niveau} << '}';
}

static std::ostream& operator<<(std::ostream& ss, action_hist action)
{
    ss << '{' << KV{"action_type", action.action_type} << ", ";

    switch (action.action_type)
    {
    case ACTION_AVANCER:
        ss << KV{"troupe_id", action.troupe_id} << ", ";
        ss << KV{"direction", action.action_dir};
        break;
    case ACTION_CONSTRUIRE:
    case ACTION_CREUSER:
        ss << KV{"position", action.action_pos};
        break;
    case ACTION_GRANDIR:
        ss << KV{"troupe_id", action.troupe_id};
        break;
    }

    return ss << '}';
}

static std::ostream& operator<<(std::ostream& ss, const troupe& troupe)
{
    return ss << '{' << KV{"id", troupe.id} << ", "
        << KV{"maman", troupe.maman} << ", "
        << KV{"canards", Vec{troupe.canards}} << ", "
        << KV{"taile", troupe.taille} << ", "
        << KV{"dir", troupe.dir} << ", "
        << KV{"inventaire", troupe.inventaire} << ", "
        << KV{"pts_actions", troupe.pts_actions} << '}';
}

// Large classes.
// ===========================================================================

template <typename T>
static std::ostream& operator<<(std::ostream& ss, Vec<T> vec)
{
    ss << '[';

    const auto& values = vec.vec;

    for (size_t i = 0; i < values.size(); i++)
    {
        ss << values[i];

        if (i < values.size() - 1)
            ss << ", ";
    }

    return ss << ']';
}

static std::ostream& operator<<(std::ostream& ss, const Map& map)
{
    ss << "{";

    ss << R"("cells": ")";

    for (int level = -1; level <= 0; level++)
        for (int x = 0; x < LARGEUR; x++)
            for (int y = 0; y < HAUTEUR; y++)
            {
                const position pos{x, y, level};
                const Case cell = map.get_cell(pos);

                ss << cell.etat.contenu;

            }

    return ss << "\"}";
}

static std::ostream& operator<<(std::ostream& ss, internal_action action)
{
    if (action.type == standard_action)
        return ss << action.action;
    assert(action.type == flag);
    return ss << '{' << KV{"action_type", "\"afficher_debug_pigeon\""} << ", "
              << KV{"pos", action.flag.pos} << ", "
              << KV{"debug_pigeon", action.flag.ctype} << '}';
}

static std::ostream& operator<<(std::ostream& ss, const PlayerInfo& player)
{
    const auto& troupes = player.troupes();
    std::vector<troupe> troupes_vec(troupes.begin(), troupes.end());

    return ss << '{' << KV{"id", player.get_key()} << ", "
              << KV{"name", Str(player.rules_player().name)} << ", "
              << KV{"score", player.rules_player().score} << ", "
              << KV{"troupes", Vec{troupes_vec}} << ", "
              << KV{"last_actions", Vec{player.get_internal_history()}} << '}';
}

static std::ostream& operator<<(std::ostream& ss, const GameState& st)
{
    const auto& players = st.players();
    std::vector<PlayerInfo> players_vec(players.begin(), players.end());

    return ss << '{' << KV{"round", "{"} << KV{"round_id", st.get_round()}
              << ", " << KV{"player_id", st.round_player_id()} << "}, "
              << KV{"map", st.get_map()} << ", "
              << KV{"players", Vec{players_vec}} << ", "
              << KV{"is_finished", st.is_finished()} << "}\n";
}

void Rules::dump_state(std::ostream& ss)
{
    ss << api_->game_state();
}

extern "C" const char* dump_state_json()
{
    // Warning: everytime this function is called, it invalidates the previous
    // return values by free-ing them.
    // This allows us to return a const char* that doesn't get invalidated as
    // soon as the function returns, though.
    static std::string s;
    std::ostringstream ss;
    ss << api->game_state();
    s = ss.str();
    return s.c_str();
}