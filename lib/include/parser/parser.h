#ifndef PARSER_H
#define PARSER_H

#include "tila.h"

/**
 * @brief Holds information about a parser
 * @field prefix A map of the prefix parsers
 * @field infix A map of the infix parsers
 * @field precedences A map of the infix parsers precedences
 * @field tokens A vector of the tokens that the parser uses
 * @field pos The position in the vector that is currently being used
 * @field src The source code of the tokens that are being parsed
 */
struct Parser {
    struct Map *prefix;
    struct Map *infix;
    struct Map *precedences;
    struct Vector *tokens;
    unsigned int pos;
    bool last_nl;
    unsigned int indent_pos;
    char *src;
    struct GC *gc;
};

/**
 * @brief A prefix parser
 */
typedef struct Node *(*PrefixParser)(struct Parser *parser,
                                     struct Token *token);
/**
 * @brief An infix parser
 */
typedef struct Node *(*InfixParser)(struct Parser *parser,
                                    struct Node *left,
                                    struct Token *token);

/**
 * @brief Creates a new parser
 * @param tokens The tokens the parser will use
 * @param src The source code that contains the tokens
 */
struct Parser *Parser_new(struct Vector *tokens, struct GC *gc, char *src);

/**
 * @brief Adds information about an infix parsers precedence
 * @param id The TokenTypeEnum type to have the precedence associated with
 * @param precedence The tokens operator precedence
 */
void Parser_add_precedence(struct Parser *parser, enum TokenTypeEnum id,
                           int precedence);

/**
 * @brief Adds a infix parser to the parser
 * @param type The TokenTypeEnum type to have the infix parser associated with
 * @param infix The infix parser function
 */
void Parser_add_infix(struct Parser *parser, enum TokenTypeEnum type,
                      InfixParser infix);

/**
 * @brief Adds a prefix parser to the parser
 * @param type The TokenTypeEnum type to have the prefix parser associated with
 * @param prefix The prefix parser function
 */
void Parser_add_prefix(struct Parser *parser, enum TokenTypeEnum type,
                       PrefixParser prefix);

/**
 * @brief Increases the required indentation by 2 spaces
 * @returns The current indentation amount
 */
int Parser_increase_indentation(struct Parser *parser);

/**
 * @brief Decreases the required indentation amount by 2 spaces
 */
void Parser_decrease_indentation(struct Parser *parser);

/**
 * @brief Does the current line have an required amount of indentation
 * @param pos The amount that is required
 * @returns True if enough, false otherwise
 */
bool Parser_has_indentation(struct Parser *parser, unsigned int pos);

/**
 * @brief Parses the tokens to form a node expression that's made out of
 *        prefixes and infixes.
 * @param precedence Infix parsers that have a higher precedence than this
 *                   will be added to the node
 */
struct Node *Parser_parse_node(struct Parser *parser, int precedence);

/**
 * @brief Is the current token of the type id
 * @param id The type that the token is checked against
 * @returns True if the type matches
 */
bool Parser_is_next(struct Parser *parser, enum TokenTypeEnum id);

/**
 * @brief If the current token is of the type id, it gets consumed
 * @param id The type that the token is checked against
 * @returns True if the type matches
 */
bool Parser_match(struct Parser *parser, enum TokenTypeEnum id);

/**
 * @brief Requires that the current token is of the type id
 * @param id The type that is required
 * @returns The token
 */
struct Token *Parser_require(struct Parser *parser, enum TokenTypeEnum id);

/**
 * @brief Returns the current token and advances to the next one
 * @returns The current token and advances to the next one
 */
struct Token *Parser_consume(struct Parser *parser);

/**
 * @brief Returns the current token
 * @returns The current token
 */
struct Token *Parser_current(struct Parser *parser);

/**
 * @brief Returns the current tokens infix parsers precedence
 * @returns The current tokens infix parsers precedence
 */
int Parser_precedence(struct Parser *parser);

/**
 * @brief Returns if the parser is done
 * @returns True if the parser is done with all of the tokens, false otherwise
 */
bool Parser_done(struct Parser *parser);

/**
 * @brief Deletes the parser
 */
void Parser_delete(struct Parser *parser);

#endif
