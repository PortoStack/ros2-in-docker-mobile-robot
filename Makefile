ENV_FILE ?= .env.local
COMPOSE = docker compose --env-file $(ENV_FILE) -f docker-compose.yml

.PHONY: help up down logs build restart shell gui clean

help:
	@echo "Available commands:"
	@echo "  make up         - Start robot stack in background"
	@echo "  make down       - Stop robot stack"
	@echo "  make logs       - View container logs in real-time"
	@echo "  make build      - Rebuild container image and start"
	@echo "  make restart    - Restart robot services"
	@echo "  make shell      - Open interactive bash shell in robot container"
	@echo "  make gui        - Start with GUI service enabled"
	@echo "  make clean      - Prune unused docker resources"

up:
	$(COMPOSE) up -d

down:
	$(COMPOSE) down

logs:
	$(COMPOSE) logs -f

build:
	$(COMPOSE) up -d --build

restart:
	$(COMPOSE) restart

shell:
	$(COMPOSE) exec robot bash

gui:
	$(COMPOSE) --profile gui up -d

clean:
	docker system prune -f
