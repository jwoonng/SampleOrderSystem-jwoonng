# Phase 3 — Repository 계층

## 목표
데이터 접근 추상화 및 JSON 영속성 구현.

## 세부 작업

### 3-1. Repository 인터페이스 정의
- `ISampleRepository`: CRUD 순수 가상 함수
- `IOrderRepository`: CRUD + 상태별 조회
- `IProductionRepository`: 큐 조회 / 추가 / 제거

### 3-2. JSON 구현체
- `JsonSampleRepository`: `data/samples.json` 읽기/쓰기
- `JsonOrderRepository`: `data/orders.json` 읽기/쓰기
- `JsonProductionRepository`: `data/production_queue.json` 읽기/쓰기
- 저장: 생성·수정·삭제 시 즉시 반영
- 로드: 애플리케이션 시작 시 전체 로드

### 3-3. JSON 파싱 유틸리티
- 외부 라이브러리 없이 수동 파싱 구현 (DataPersistence PoC 참조)
- 임시 파일(`.tmp`) 기반 원자적 저장

## 완료 기준
- Repository를 통한 CRUD 동작 확인
- 앱 재시작 후 데이터 유지 확인
