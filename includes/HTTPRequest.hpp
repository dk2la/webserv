//
// Created on 2/4/21 by
// Shonna Jakku
// Fenix Lavon
// Freely Kathryn
//  Copyright (c) 2021 RaevkaTuliskiyPryanikNogotochki All rights reserved.
//

#pragma once

#include <iostream>
#include <map>
#include <vector>
#include <sstream>
#include "utils.hpp"

class HTTPRequest {
private:
	std::map<std::string, std::string>	_request_params;
	char *		_method;
	char *		_http_path;
	char *		_http_query;
	char *		_version_http;

	char *	_body;
	char *	_request;


	size_t _request_capacity;
	size_t 		_request_size;
	size_t 		_body_size;
	size_t		_body_capacity;
	size_t	_hex_size;

	int			_stage;
	std::string _status_code;
public:

	explicit HTTPRequest();
	~HTTPRequest();
	void		parse_request_http(char * buf, int bytes);
	void		parseFirstLine(char * line);

	char *	getStr(size_t pos);
	void	takeHeader(char *header);
	int		parseBodyRequest();

	const char *						getMethod() const { return _method; };

	const char *						getPath() const {return _http_path; }
	const char *						getHttpQuery() const { return _http_query; }

	int getBodySize() const;;
	const char *						getVersionHTTP() const { return _version_http; };
	const char *						getContentType();
	int									getParsingStage() const { return _stage; }
	std::map<std::string, std::string>	getHeaders() { return _request_params; }
	const char*							getBody() { return _body; }

	char *getArgument(char *dst, int start);
	void								setMethod(char * method);
	void								setPath(char * path);
	void								setVersionHTTP(char * version_http);

	void			addBufToRequest(char *buf, int buf_size);
	void			addBufToBody(char *buf, int buf_size);
	void			ft_erase_request(int size);
	void			ft_erase_body(int size);
	void			clear();
};
