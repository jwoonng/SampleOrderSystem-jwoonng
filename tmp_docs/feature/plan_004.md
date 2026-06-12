# Phase 4 — Service 계층 : 시료 관리

## 목표
`prd.md` 3-2 시료 관리 비즈니스 로직 구현.

## 세부 작업

### 4-1. SampleService 구현
- `registerSample()`: 시료 등록, 중복 ID 검증
- `getAllSamples()`: 전체 시료 목록 반환
- `findSampleById()`: ID로 시료 조회
- `searchSamples()`: 이름 등 속성으로 시료 검색

### 4-2. 유효성 검사
- 시료 ID 형식 검증 (S-XXX)
- 수율 범위 검증 (0.0 초과 ~ 1.0 이하)
- 평균 생산시간 양수 검증

## 완료 기준
- 시료 등록 / 조회 / 검색 동작 확인
- 유효하지 않은 입력 시 예외 발생 확인
