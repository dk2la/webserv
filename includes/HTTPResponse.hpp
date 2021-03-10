//
// Created on 2/4/21 by
// Shonna Jakku
// Fenix Lavon
// Freely Kathryn
//  Copyright (c) 2021 RaevkaTuliskiyPryanikNogotochki All rights reserved.
//

#pragma once

#include "HTTPRequest.hpp"
#include "HTTPRequest.hpp"
#include <iostream>
#include <iterator>

enum { count_status_code = 13, };

class HTTPResponse {

public:
	/* Constructor */
	explicit HTTPResponse();

	/* Destructor */
	~HTTPResponse() { free(_buf_response); free(_body); free(_headers_cgi); };

	/* Static massive */
	static const std::string		message_phrases[count_status_code][2];

	/* Getters */
	static std::string				getMessagePhrase(const std::string& code);
	std::string						generateErrorPage();
	char *							getResponse() const { return _buf_response; }
	int 							getBodySize() const;

	/* Setters */
	void							setStatusCode(const std::string& status) { _status_code = status; }
	void							setBody(std::pair<char *, int> buf);
	void							setCgiHeaders(char* headers) { free(_headers_cgi); _headers_cgi = ft_strdup(headers);  }

	/* Modifiers */
	void							generateResponse(HTTPRequest* request);
	void							clear();

private:
	std::string 	_status_code;
	char *			_buf_response;
	char *			_body;
	char *			_headers_cgi;
	size_t 			_body_size;
	size_t 			_header_size;
};