# Only_Won

![](https://github.com/joonyle99/Only_Won/assets/67359781/46d6fa61-afcc-4954-83b7-58b47548cf95)

* 프로젝트 이름 : `Only Won`
* 장르 : `캐주얼` / `경쟁` / `파티` / `전략` / `로컬 멀티` / `컨트롤러` / `탑뷰`
* 개발 환경 : `Visual Studio 2022` / `Direct2D` / `C++`
* 프로젝트 목적 : `DirectX 2D를 이용한 게임 엔진 프레임워크 학습`
* 제작 기간 : `2023.08.07` ~ `2023.08.25`
* 담당 파트 :
  1. `객체 모델을 위한 계층 구조 설계 및 개발`
     - Object, GameObject, Component ...
  2. `물리 / 충돌 기능 개발`
     - 물리 : 간단한 Rigidbody, 가속도를 이용한 부드러운 움직임
     - 충돌 콜라이더 : boxCollider, circleCollider, AABB, OBB
     - 특정 콜라이더와 충돌 가능한 LayerMask 기능
  3. `Xbox 컨트롤러 InputManager 개발`
     - 4개의 컨트롤러 동시 입력
  4. `스킬 아이템 구현`
  5. `기차 시스템 개발`
  6. `각종 Util 클래스`

<details>
<summary>느낀점</summary>
<div markdown="1">

<br>

__개발 관점__  
: Win32API만으로는 한계가 있던 엔진 제작이, 게임 엔진 제작에 최적화된 그래픽스 API DirectX 11을 이용하면서 성능 향상, 그래픽 품질 개선, 그리고 개발 효율성의 증가를 체감할 수 있었습니다. 하드웨어 가속, 프레임 최적화, 변환 행렬, 등은 게임의 성능을 향상시켜 주었고, 팩토리와 같은 객체는 Image, Geometry, RenderTarget과 같은 Direct2D의 리소스 및 객체의 생성을 도와주며 개발 효율성을 향상시켜 주었습니다. 

</div>
</details>

#### 기술 문서 1 : [https://github.com/joonyle99/Only_Won/discussions/4 ](https://github.com/joonyle99/Only_Won/discussions/4)
#### 기술 문서 2 : [https://github.com/joonyle99/Only_Won/discussions/5 ](https://github.com/joonyle99/Only_Won/discussions/5)
#### 플레이 영상 : [https://www.youtube.com/watch?v=rq6Tg1pvA_8](https://www.youtube.com/watch?v=rq6Tg1pvA_8)
#### 스토브 인디 데모 : [https://store.onstove.com/ko/games/2391](https://store.onstove.com/ko/games/2391)

![](https://github.com/joonyle99/Only_Won/assets/67359781/9ac923f9-7aeb-4ebf-943b-d4fb657ff718)
