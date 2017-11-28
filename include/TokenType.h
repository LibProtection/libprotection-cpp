#ifndef PROTECTION_TOKENTYPE_H
#define PROTECTION_TOKENTYPE_H

namespace protection {
enum class TokenType {

  ErrorTokenType,

  FilePathError,
  FilePathDissallowedSymbol,
  FilePathDeviceID,
  FilePathFSEntryName,
  FilePathNTFSAttribute,
  FilePathSeparator,

  UrlError,
  UrlSeparator,
  UrlScheme,
  UrlAuthorityEntry,
  UrlPathEntry,
  UrlQueryEntry,
  UrlFragment,

  // Non-terminals
  UrlSchemeCtx,
  UrlAuthorityCtx,
  UrlPathCtx,
  UrlQueryCtx,
  UrlFragmentCtx

};
}

#endif // PROTECTION_TOKENTYPE_H
