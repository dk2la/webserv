//
// Created on 2/5/21 by
// Shonna Jakku
// Fenix Lavon
// Freely Kathryn
//  Copyright (c) 2021 RaevkaTuliskiyPryanikNogotochki All rights reserved.
//

#include "HTTPRequest.hpp"
#include "fcntl.h"

HTTPRequest::HTTPRequest(): _stage(false), _body_size(0), _hex_size(-1) {
	_request = ft_strdup("");
	_request_size = 0;
	_request_capacity = 0;
	_body_capacity = 0;
	_body = ft_strdup("");
}

HTTPRequest::~HTTPRequest() {
	free(_request);
	free(_body);
	free(_method);
	free(_version_http);
	free(_http_path);
	free(_http_query);
}

char *HTTPRequest::getArgument(char *dst, int start) {
	char *tmp = ft_substr(dst, start, ft_strlen(dst));
	char *result = ft_strtrim(tmp, " \t");
	free(tmp);
	return result;
}

char * HTTPRequest::getStr(size_t pos) {
	char *result = ft_substr(_request, 0, pos);
	ft_erase_request(pos + 2);
	return result;
}

void HTTPRequest::takeHeader(char *header) {
	size_t pos = ft_strchr(header, ':');
	if (pos == (size_t)-1)
		throw std::string("400");

	char *tmp = ft_substr(header, 0, pos - 1);
	char *value = getArgument(header, pos);
	_request_params[tmp] = value;
	free(value);
	free(tmp);
	free(header);
}

void HTTPRequest::parse_request_http(char * buf, int bytes) {
	addBufToRequest(buf, bytes);
	size_t pos;
	while (_request_size != 0 && _stage != 3) {
		if (_stage == 0) {
			if ((pos = ft_find(_request, "\r\n")) != (size_t) -1)
				parseFirstLine(getStr(pos));
			else break;
		}
		else if (_stage == 1) {
			if ((pos = ft_find(_request, "\r\n")) != (size_t)-1 && pos != 0)
				takeHeader(getStr(pos));
			else if (pos == 0) {
				ft_erase_request(2);
				if (_request_params.count("Host") == 0)
					throw std::string ("400");
				if (ft_compare(_method, "PUT") || ft_compare(_method, "POST"))
					if (_request_params.count("Content-Length") == 0 && _request_params.count("Transfer-Encoding") == 0)
						throw std::string ("400");
					else
						_stage = 2;
				else
					_stage = 3;
			}
			else break;
		} else if (_stage == 2) {
			if (parseBodyRequest() == 1)
				_stage = 3;
			else
				break;
		}
	}
}

int HTTPRequest::parseBodyRequest() {
	if (_request_params.count("Content-Length")) {
		size_t size = ft_atoi(_request_params["Content-Length"].c_str());
		addBufToBody(_request, size);
		if (_body_size > size)
			ft_erase_body(_body_size - size);
		return 1;
	} else if (_request_params.count("Transfer-Encoding")) {
		size_t pos;
		while (_request) {
			if ((pos = ft_find(_request, "\r\n")) != (size_t)-1) {
				if (_hex_size == (size_t)-1) {
					_hex_size = ft_atoi_chunk(getStr(pos));
				} else if (_hex_size != 0) {
					addBufToBody(_request, _hex_size);
					ft_erase_request(pos + 2);
					_hex_size = -1;
				} else return 1;
			} else return 0;
		}
	}
	return 0;
}

void HTTPRequest::parseFirstLine(char *line) {
	char ** dst = ft_split(line, ' ');
	for (int i = 0; dst[i] != nullptr; ++i) {
		if (i == 0) {
			if (ft_compare(dst[i], "GET") || ft_compare(dst[i], "POST") || ft_compare(dst[i], "PUT") || ft_compare(dst[i], "HEAD"))
				setMethod(dst[i]);
			else throw std::string("400");
		}
		else if (i == 1)
			setPath(dst[i]);
		else {
			if (ft_compare(dst[i], "HTTP/1.1") || ft_compare(dst[i], "HTTP/1.0"))
				setVersionHTTP(dst[i]);
			else std::cerr << "Error set version HTTP/1.1" << std::endl;
		}
	}
	free(line);
	++_stage;
}

void HTTPRequest::setMethod(char *method) {
	_method = method;
}

void HTTPRequest::setPath(char * path) {
	size_t pos = ft_strchr(path, '?');
	if (pos == (size_t)-1) {
		_http_path = path;
		_http_query = ft_strdup("");
	} else {
		_http_path = ft_substr(path, 0, pos - 1);
		_http_query = ft_substr(path, pos + 2, ft_strlen(path));
	}
}

void HTTPRequest::setVersionHTTP(char *version_http) {
	_version_http = version_http;
}

int HTTPRequest::getBodySize() const {
	return _body_size;
}

const char *HTTPRequest::getContentType() {
	std::map<std::string, std::string>::iterator it = _request_params.find("Content-Type");
	if (it != _request_params.end())
		return it->second.c_str();
	return nullptr;
}

void HTTPRequest::addBufToRequest(char *buf, int buf_size) {
	if (_request_size + buf_size >= _request_capacity) {
		_request_capacity = (_request_size + buf_size) * 2;
		char *_realloc_request = _request;
		_request = (char *)malloc(_request_capacity);
		ft_memcpy(_request, _realloc_request, _request_size);
		free(_realloc_request);
	}
	for (int i = 0; i < buf_size; ++i, ++_request_size)
		_request[_request_size] = buf[i];
	_request[_request_size] = '\0';
}


void HTTPRequest::ft_erase_request(int size) {
	_request_size -= size;
	std::memmove(_request, _request + size, _request_size);
	_request[_request_size] = '\0';
}

void HTTPRequest::addBufToBody(char *buf, int buf_size) {
	if (_body_size + buf_size >= _body_capacity) {
		_body_capacity = (_body_size + buf_size) * 2;
		char *_realloc_body = _body;
		_body = (char *)malloc(_body_capacity);
		ft_memcpy(_body, _realloc_body, _body_size);
		free(_realloc_body);
	}
	for (int i = 0; i < buf_size; ++i, ++_body_size)
		_body[_body_size] = buf[i];
	_body[_body_size] = '\0';
}

void HTTPRequest::ft_erase_body(int size) {
	std::memmove(_body, _body + size, _body_size);
	_body_size -= size;
}

void HTTPRequest::clear() {
	free(_request);
	free(_body);
	_request = ft_strdup("");
	_body = ft_strdup("");
	_request_size = 0;
	_request_capacity = 0;
	_body_capacity = 0;
	_stage = false;
	_body_size = 0;
	_hex_size = -1;
	_status_code.clear();
}




