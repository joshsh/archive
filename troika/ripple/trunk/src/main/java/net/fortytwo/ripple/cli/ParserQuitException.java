package net.fortytwo.ripple.cli;

/**
*  A trivial exception which is used to break out of the ANTLR-generated lexer
*  and parser, which never match end-of-input.
*/
public class ParserQuitException extends RuntimeException {}

// kate: tab-width 4
