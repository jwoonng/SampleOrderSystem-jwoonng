# Phase 1 — 프로젝트 기반 구조

## 목표
빌드 가능한 빈 뼈대 완성 및 개발 환경 구성.

## 세부 작업

### 1-1. 디렉터리 및 파일 구조 생성
- `src/model/`, `src/view/`, `src/controller/`, `src/service/`, `src/repository/`, `src/util/` 생성
- `data/`, `tests/` 생성
- 각 디렉터리에 빈 `.gitkeep` 또는 placeholder 헤더 생성

### 1-2. 진입점 구성
- `main.cpp` 생성
- 한글 인코딩 설정: `_setmode` 3개 적용
- "시스템 초기화 완료" 출력 후 정상 종료 확인

### 1-3. 빌드 설정
- `.vcxproj`에 신규 파일 등록
- `/utf-8` 컴파일러 옵션 확인
- Debug / Release x64 빌드 성공, 경고 0건 확인

## 완료 기준
- Debug / Release 빌드 성공
- 콘솔에 한글 출력 정상 확인
