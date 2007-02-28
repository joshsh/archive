package net.fortytwo.ripple.ci;

/**
 *  A trivial exception which is used to break out of the ANTLR-generated lexer
 *  and parser, which never match end-of-input.
 */
public class ParserQuitException extends RuntimeException {}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
